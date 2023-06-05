#include "Server.h"
#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "../network.h"

namespace beetr {
    namespace server {

        absl::Status Server::Start() {
            auto fd = socket(AF_INET, SOCK_STREAM, 0);

            if (fd < 0) {
                return absl::InternalError("could not open socket");
            }

            int val = 1;
            setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof val);

            sockaddr_in addr = {
                    .sin_family = AF_INET,
                    .sin_port = ntohs(port),
            };
            addr.sin_addr.s_addr = ntohl(0);

            auto rv = bind(fd, (const sockaddr *) &addr, sizeof addr);

            if (rv) {
                return absl::InternalError(absl::StrFormat("could not bind to socket: %s", strerror(errno)));
            }

            rv = listen(fd, SOMAXCONN);

            if (rv) {
                return absl::InternalError("could not listen");
            }

            while (true) {
                sockaddr_in client_addr = {};

                auto len = sizeof client_addr;

                int connfd = accept(fd, (sockaddr *) &client_addr, reinterpret_cast<socklen_t *>(&len));

                if (connfd < 0) {
                    LOG(WARNING) << "got error on trying to accept connection: " << connfd << std::endl;
                    continue;
                }

                while (true) {
                    auto err = process_one_request(connfd);
                    if (!err.ok()) {
                        LOG(ERROR) << "got error on parsing one request " << err.message() << std::endl;
                        break;
                    }
                }

                close(connfd);
            }

            return absl::OkStatus();
        }

        absl::Status Server::process_one_request(int connfd) {
            char rbuf[4 + k_max_bytes + 1] = {};
            uint32_t len = 0;

            auto err = network::io::read_full(connfd, rbuf, sizeof len);

            if (!err.ok()) {
                return err;
            }

            memcpy(&len, rbuf, sizeof len);

            if (len > k_max_bytes) {
                return absl::InternalError(absl::StrFormat("message too long: %d", len));
            }

            err = network::io::read_full(connfd, rbuf + sizeof len, len);

            if (!err.ok()) {
                return err;
            }

            rbuf[4 + len] = '\0';

            LOG(INFO) << "Client says " << rbuf + 4 << std::endl;

            std::string write_message = "world";

            char wbuf[write_message.size() + sizeof len];

            len = write_message.size();

            memcpy(wbuf, &len, sizeof len);
            memcpy(wbuf + sizeof len, write_message.c_str(), len);

            return network::io::write_full(connfd, wbuf, len + sizeof len);
        }
    }
}