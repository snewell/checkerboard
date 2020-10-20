#ifndef CHECKERBOARD_DOMAIN_HPP
#define CHECKERBOARD_DOMAIN_HPP 1

#include <sys/socket.h>
#include <sys/types.h>

namespace checkerboard
{
    enum class Domain
    {
        // inet = AF_INET,
        // inet6 = AF_INET6
    };

    constexpr Domain inet = Domain(AF_INET);
    constexpr Domain inet6 = Domain(AF_INET6);
    constexpr Domain unix_socket = Domain(AF_UNIX);
} // namespace checkerboard

#endif
