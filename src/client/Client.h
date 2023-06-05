#ifndef BEETR_CLIENT_H
#define BEETR_CLIENT_H

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "absl/status/status.h"

namespace beetr {
namespace client {
    constexpr int k_max_bytes = 4096;
    class Client {
    private:
        unsigned short port;
    public:
        Client(unsigned short port): port(port) {}
        absl::Status Start();
    };
}
}


#endif //BEETR_CLIENT_H
