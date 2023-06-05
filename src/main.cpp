#include <thread>
#include "server/Server.h"
#include "client/Client.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"

ABSL_FLAG(uint16_t, port, 1225, "port");
ABSL_FLAG(bool, server, true, "server");

int main(int argc, char **argv) {
    absl::ParseCommandLine(argc, argv);
//    absl::InitializeLog();

    unsigned short port = absl::GetFlag(FLAGS_port);

    if (absl::GetFlag(FLAGS_server)) {
        auto s = beetr::server::Server(port);

        auto err = s.Start();
        if (!err.ok()) {
            LOG(ERROR) << err.message() << std::endl;
        }
    } else {
        auto c = beetr::client::Client(port);

        if (auto err = c.Start(); !err.ok()) {
            LOG(ERROR) << err.message() << std::endl;
        }

    }

    return 0;
}
