#ifndef BEETR_SERVER_H
#define BEETR_SERVER_H

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "absl/status/status.h"

namespace beetr::server {

        constexpr int k_max_bytes = 4096;

        class Server {
        private:
            unsigned short port;
        public:
            explicit Server(unsigned short port) : port(port) {}

            absl::Status Start();

            absl::Status process_one_request(int connfd);

        };
    }  // beetr

#endif //BEETR_SERVER_H
