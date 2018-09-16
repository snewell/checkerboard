#ifndef CHECKERBOARD_TYPE_HPP
#define CHECKERBOARD_TYPE_HPP 1

#include <sys/socket.h>
#include <sys/types.h>

namespace checkerboard
{
    enum class Type
    {
    };

    constexpr Type stream = Type(SOCK_STREAM);
    constexpr Type datagram = Type(SOCK_DGRAM);
} // namespace checkerboard

#endif
