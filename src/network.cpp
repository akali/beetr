#include "absl/status/status.h"
#include "absl/strings/str_format.h"
#include "absl/log/log.h"

namespace network::io {

    absl::Status read_full(int fd, char *buf, size_t n) {
        char *head = buf;
        while (n > 0) {
            auto rv = read(fd, buf, n);

            absl::Status err = absl::OkStatus();

            if (rv == 0) {
                err = absl::InternalError(absl::StrFormat("got unexpected EOF during read, actually expected at most %d bytes: %d", n, rv));
            } else if (rv < 0) {
                err = absl::InternalError(absl::StrFormat("got internal error: %s", strerror(errno)));
            } else if (rv > n) {
                err = absl::InternalError(absl::StrFormat("got unexpectedly big response: want at most %d, got %d", n, rv));
            }

            if (!err.ok()) {
                return err;
            }

            n -= static_cast<size_t>(rv);
            buf += rv;
        }

        return absl::OkStatus();
    }

    absl::Status write_full(int fd, const char *buf, size_t n) {
        while (n > 0) {
            auto rv = write(fd, buf, n);

            absl::Status err = absl::OkStatus();

            if (rv == 0) {
                err = absl::InternalError(absl::StrFormat("got unexpected EOF during write, actually expected at most %d bytes: %d", n, rv));
            } else if (rv < 0) {
                err = absl::InternalError(absl::StrFormat("got internal error: %s", strerror(errno)));
            } else if (rv > n) {
                err = absl::InternalError(absl::StrFormat("got unexpectedly big response: want at most %d, got %d", n, rv));
            }

            if (!err.ok()) {
                return err;
            }

            n -= static_cast<size_t>(rv);
            buf += rv;
        }

        return absl::OkStatus();
    }
}