#ifndef CHECKERBOARD_TYPE_HPP
#define CHECKERBOARD_TYPE_HPP 1

#include <sys/types.h>
#include <sys/socket.h>

namespace checkerboard
{
    enum class Type
    {
        stream = SOCK_STREAM,
        datagram = SOCK_DGRAM
    };
}

#endif
