#ifndef CHECKERBOARD_FNS_HPP
#define CHECKERBOARD_FNS_HPP 1

#include <checkerboard/address.hpp>
#include <checkerboard/socket.hpp>

namespace checkerboard
{
    template <Domain DOMAIN, Type TYPE>
    BoundSocket<DOMAIN, TYPE> bind(Socket<DOMAIN, TYPE> && socket, Address<DOMAIN> const & address)
    {
        ::bind(socket.socket(), address.sockaddr(),
               Address<DOMAIN>::sockaddr_size);
        return BoundSocket<DOMAIN, TYPE>{std::move(socket)};
    }

    template <Domain DOMAIN, Type TYPE>
    ListeningSocket<DOMAIN, TYPE> listen(BoundSocket<DOMAIN, TYPE> && socket, unsigned int backlog)
    {
        ::listen(socket.socket(), backlog);
        return ListeningSocket<DOMAIN, TYPE>{std::move(socket)};
    }
}

#endif
