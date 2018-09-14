#ifndef CHECKERBOARD_DOMAIN_HPP
#define CHECKERBOARD_DOMAIN_HPP 1

#include <sys/types.h>
#include <sys/socket.h>

namespace checkerboard
{
    enum class Domain
    {
        inet = AF_INET,
        inet6 = AF_INET6
    };
}

#endif
