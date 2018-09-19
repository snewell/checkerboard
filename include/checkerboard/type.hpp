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

    template <Type TYPE>
    struct accepts_connections;

    template <Type TYPE>
    static constexpr auto accepts_connections_v =
        accepts_connections<TYPE>::value;

    template <Type TYPE>
    struct is_connectionless;

    template <Type TYPE>
    static constexpr auto is_connectionless_v = is_connectionless<TYPE>::value;

    template <>
    struct accepts_connections<stream>
    {
        static constexpr bool value = true;
    };

    template <>
    struct is_connectionless<datagram>
    {
        static constexpr bool value = true;
    };
} // namespace checkerboard

#endif
