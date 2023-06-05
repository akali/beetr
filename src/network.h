#ifndef BEETR_NETWORK_H
#define BEETR_NETWORK_H

#include "absl/status/status.h"

namespace network::io {
    absl::Status read_full(int fd, char *buf, size_t n);
    absl::Status write_full(int fd, const char *buf, size_t n);
}

#endif //BEETR_NETWORK_H
