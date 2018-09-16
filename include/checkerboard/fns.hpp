#ifndef CHECKERBOARD_FNS_HPP
#define CHECKERBOARD_FNS_HPP 1

#include <errno.h>

#include <stdexcept>
#include <tuple>

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

    template <Domain DOMAIN, Type TYPE>
    ConnectedSocket<DOMAIN, TYPE> connect(Socket<DOMAIN, TYPE> && socket,
                                          Address<DOMAIN> const & address)
    {
        if(::connect(socket.socket(), address.sockaddr(),
                     Address<DOMAIN>::sockaddr_size) != 0)
        {
            inner::emit_exception(errno);
        }
        return ConnectedSocket<DOMAIN, TYPE>{std::move(socket)};
    }

    template <Domain DOMAIN, Type TYPE>
    std::tuple<ConnectedSocket<DOMAIN, TYPE>, Address<DOMAIN>>
    accept(ListeningSocket<DOMAIN, TYPE> & socket)
    {
        typename Address<DOMAIN>::sockaddr_type sa;
        auto len = Address<DOMAIN>::sockaddr_size;

        auto new_socket = ::accept(socket.socket(),
                                   reinterpret_cast<::sockaddr *>(&sa), &len);
        if(new_socket < 0)
        {
            inner::emit_exception(errno);
        }
        return std::make_tuple(
            ConnectedSocket<DOMAIN, TYPE>{Socket<DOMAIN, TYPE>{new_socket}},
            Address<DOMAIN>{sa});
    }

    namespace inner
    {
        template <Domain DOMAIN, Type TYPE, typename DATA>
        std::size_t real_send(ConnectedSocket<DOMAIN, TYPE> & socket,
                              DATA const * data, std::size_t size)
        {
            // TODO: check errors and retry if it's a transitive error
            auto ret = ::send(socket.socket(), data, size, 0);
            if(ret < 0)
            {
                inner::emit_exception(errno);
            }
            return ret;
        }
    } // namespace inner

    template <Domain DOMAIN, Type TYPE>
    std::size_t send(ConnectedSocket<DOMAIN, TYPE> & socket,
                     std::uint8_t const * data, std::size_t size)
    {
        return inner::real_send(socket, data, size);
    }

    template <Domain DOMAIN, Type TYPE, std::size_t SIZE>
    std::size_t send(ConnectedSocket<DOMAIN, TYPE> & socket,
                     std::uint8_t const (&data)[SIZE])
    {
        return inner::real_send(socket, data, SIZE);
    }

    template <Domain DOMAIN, Type TYPE>
    std::size_t send(ConnectedSocket<DOMAIN, TYPE> & socket,
                     std::int8_t const * data, std::size_t size)
    {
        return inner::real_send(socket, data, size);
    }

    template <Domain DOMAIN, Type TYPE, std::size_t SIZE>
    std::size_t send(ConnectedSocket<DOMAIN, TYPE> & socket,
                     std::int8_t const (&data)[SIZE])
    {
        return inner::real_send(socket, data, SIZE);
    }

    namespace inner
    {
        template <Domain DOMAIN, Type TYPE, typename DATA>
        std::size_t real_recv(ConnectedSocket<DOMAIN, TYPE> & socket,
                              DATA * data, std::size_t size)
        {
            // TODO: check errors and retry if it's a transitive error
            auto ret = ::recv(socket.socket(), data, size, 0);
            if(ret < 0)
            {
                inner::emit_exception(errno);
            }
            return ret;
        }
    } // namespace inner

    template <Domain DOMAIN, Type TYPE>
    std::size_t recv(ConnectedSocket<DOMAIN, TYPE> & socket,
                     std::uint8_t * data, std::size_t size)
    {
        return inner::real_recv(socket, data, size);
    }

    template <Domain DOMAIN, Type TYPE, std::size_t SIZE>
    std::size_t recv(ConnectedSocket<DOMAIN, TYPE> & socket,
                     std::uint8_t (&data)[SIZE])
    {
        return inner::real_recv(socket, data, SIZE);
    }

    template <Domain DOMAIN, Type TYPE>
    std::size_t recv(ConnectedSocket<DOMAIN, TYPE> & socket, std::int8_t * data,
                     std::size_t size)
    {
        return inner::real_recv(socket, data, size);
    }

    template <Domain DOMAIN, Type TYPE, std::size_t SIZE>
    std::size_t recv(ConnectedSocket<DOMAIN, TYPE> & socket,
                     std::int8_t (&data)[SIZE])
    {
        return inner::real_recv(socket, data, SIZE);
    }
} // namespace checkerboard

#endif
