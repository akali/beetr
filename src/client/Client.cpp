#include "Client.h"
#include "absl/status/status.h"
#include "absl/log/log.h"
#include "../network.h"
#include "absl/strings/str_format.h"

absl::Status beetr::client::Client::Start() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) {
        return absl::InternalError("failed to create a socket");
    }

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(port);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);

    int rv = connect(fd, (const sockaddr*)&addr, sizeof addr);
    if (rv) {
        return absl::InternalError(absl::StrFormat("failed to connect to socket: %s", strerror(errno)));
    }

    std::string msg = "hello";
    uint32_t len = msg.size();

    char wbuf[msg.size() + sizeof len];

    memcpy(wbuf, &len, sizeof len);
    memcpy(wbuf + sizeof len, msg.c_str(), msg.size());

    auto err = network::io::write_full(fd, wbuf, sizeof wbuf);

    if (!err.ok()) {
        return err;
    }

    char rbuf[k_max_bytes + 1 + sizeof len];

    err = network::io::read_full(fd, rbuf, sizeof len);

    if (!err.ok()) {
        return err;
    }

    memcpy(&len, rbuf, sizeof len);

    if (len > k_max_bytes) {
        return absl::InternalError(absl::StrFormat("message too long: %d", len));
    }

    err = network::io::read_full(fd, rbuf + sizeof len, len);

    if (!err.ok()) {
        return err;
    }

    rbuf[len + sizeof len] = '\0';

    LOG(INFO) << "Server says " << rbuf + sizeof len << std::endl;

    close(fd);

    return absl::OkStatus();
}
