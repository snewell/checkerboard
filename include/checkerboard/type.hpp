#ifndef CHECKERBOARD_TYPE_HPP
#define CHECKERBOARD_TYPE_HPP 1

#include <sys/types.h>
#include <sys/socket.h>

namespace checkerboard
{
    enum class Type { };

    constexpr Type stream = Type(SOCK_STREAM);
    constexpr Type datagraph = Type(SOCK_DGRAM);
}

#endif
