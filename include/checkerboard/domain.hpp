#ifndef CHECKERBOARD_DOMAIN_HPP
#define CHECKERBOARD_DOMAIN_HPP 1

#include <sys/types.h>
#include <sys/socket.h>

namespace checkerboard
{
    enum class Domain
    {
        //inet = AF_INET,
        //inet6 = AF_INET6
    };

    constexpr Domain inet = Domain(AF_INET);
    constexpr Domain inet6 = Domain(AF_INET6);
}

#endif
