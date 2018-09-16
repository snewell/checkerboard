#ifndef CHECKERBOARD_FNS_HPP
#define CHECKERBOARD_FNS_HPP 1

#include <errno.h>

#include <stdexcept>

#include <checkerboard/address.hpp>
#include <checkerboard/socket.hpp>

namespace checkerboard
{
    namespace inner
    {
        inline void emit_exception(int error)
        {
            throw std::runtime_error{::strerror(error)};
        }
    } // namespace inner

    template <Domain DOMAIN, Type TYPE>
    BoundSocket<DOMAIN, TYPE> bind(Socket<DOMAIN, TYPE> && socket,
                                   Address<DOMAIN> const & address)
    {
        if(::bind(socket.socket(), address.sockaddr(),
                  Address<DOMAIN>::sockaddr_size) != 0)
        {
            inner::emit_exception(errno);
        }
        return BoundSocket<DOMAIN, TYPE>{std::move(socket)};
    }

    template <Domain DOMAIN, Type TYPE>
    ListeningSocket<DOMAIN, TYPE> listen(BoundSocket<DOMAIN, TYPE> && socket,
                                         unsigned int backlog)
    {
        if(::listen(socket.socket(), backlog) != 0)
        {
            inner::emit_exception(errno);
        }
        return ListeningSocket<DOMAIN, TYPE>{std::move(socket)};
    }
} // namespace checkerboard

#endif
