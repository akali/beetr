cmake_minimum_required(VERSION 3.16.3)
project(beetr)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(third_party/abseil-cpp)

set(DEBUG 1)

add_executable(beetr src/main.cpp src/server/Server.cpp src/server/Server.h src/client/Client.cpp src/client/Client.h src/network.cpp src/network.h)
set(ABSL_DEPS
        absl::base
        absl::core_headers
        absl::absl_check
        absl::absl_log
        absl::check
        absl::die_if_null
        absl::flags
        absl::flags_commandlineflag
        absl::flags_marshalling
        absl::flags_parse
        absl::flags_usage
        absl::log
        absl::log_flags
        absl::log_globals
        absl::log_initialize
        absl::log_internal_message
        absl::cord
        absl::random_random
        absl::raw_hash_set
        absl::hash
        absl::leak_check
        absl::memory
        absl::meta
        absl::stacktrace
        absl::status
        absl::statusor
        absl::str_format
        absl::strings
        absl::synchronization
        absl::time
        absl::any
        )

target_link_libraries(beetr PRIVATE pthread ${ABSL_DEPS})
