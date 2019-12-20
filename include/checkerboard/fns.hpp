#ifndef CHECKERBOARD_FNS_HPP
#define CHECKERBOARD_FNS_HPP 1

#include <cerrno>
#if __cplusplus >= 201703L
#include <cstddef> // needed for std::byte
#endif

#include <stdexcept>
#include <tuple>

#include <checkerboard/address.hpp>
#include <checkerboard/socket.hpp>

namespace checkerboard
{
    namespace inner
    {
        template <typename BYTE>
        struct is_byte
        {
            static constexpr bool value =
                std::is_integral<BYTE>::value && (sizeof(BYTE) == 1);
        };

#if __cplusplus >= 201703L
        // std::byte isn't considered an integral type, so we need to specialize
        template <>
        struct is_byte<std::byte>
        {
            static constexpr bool value = true;
        };
#endif

        template <typename BYTE>
        static constexpr auto is_byte_v = is_byte<BYTE>::value;

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
        static_assert(accepts_connections_v<TYPE>,
                      "listen is only available for connection-based types");

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

        template <typename SOCKET, Domain DOMAIN, typename DATA>
        std::size_t real_send_to(SOCKET & socket,
                                 Address<DOMAIN> const & destination,
                                 DATA const * data, std::size_t size)
        {
            static_assert(connectionless_send_v<SOCKET>,
                          "send_to requires a connectionless Socket");
            static_assert(is_connectionless_v<SOCKET::type>,
                          "send_to requires a connectionless type");

            // TODO: check errors and retry if it's a transitive error
            auto ret =
                ::sendto(socket.socket(), data, size, 0, destination.sockaddr(),
                         Address<DOMAIN>::sockaddr_size);
            if(ret < 0)
            {
                inner::emit_exception(errno);
            }
            return ret;
        }
    } // namespace inner

    template <Domain DOMAIN, Type TYPE, typename BYTE>
    std::size_t send(ConnectedSocket<DOMAIN, TYPE> & socket, BYTE const * data,
                     std::size_t size)
    {
        static_assert(inner::is_byte_v<BYTE>,
                      "Send buffer must be a byte sequence");
        return inner::real_send(socket, data, size);
    }

    template <Domain DOMAIN, Type TYPE, typename BYTE, std::size_t SIZE>
    std::size_t send(ConnectedSocket<DOMAIN, TYPE> & socket,
                     BYTE const (&data)[SIZE])
    {
        static_assert(inner::is_byte_v<BYTE>,
                      "Send buffer must be a byte sequence");
        return inner::real_send(socket, data, SIZE);
    }

    template <typename SOCKET, Domain DOMAIN, typename BYTE>
    std::size_t send_to(SOCKET & socket, Address<DOMAIN> const & destination,
                        BYTE const * data, std::size_t size)
    {
        static_assert(inner::is_byte_v<BYTE>,
                      "Send buffer must be a byte sequence");
        return inner::real_send_to(socket, destination, data, size);
    }

    template <typename SOCKET, Domain DOMAIN, typename BYTE, std::size_t SIZE>
    std::size_t send_to(SOCKET & socket, Address<DOMAIN> const & destination,
                        BYTE const (&data)[SIZE])
    {
        static_assert(inner::is_byte_v<BYTE>,
                      "Send buffer must be a byte sequence");
        return inner::real_send_to(socket, destination, data, SIZE);
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

        template <typename SOCKET, typename DATA,
                  Domain DOMAIN = SOCKET::domain>
        std::tuple<std::size_t, Address<DOMAIN>>
        real_recv_from(SOCKET & socket, DATA * data, std::size_t size)
        {
            static_assert(connectionless_send_v<SOCKET>,
                          "send_to requires a connectionless Socket");
            static_assert(is_connectionless_v<SOCKET::type>,
                          "send_to requires a connectionless type");

            typename Address<DOMAIN>::sockaddr_type sa;
            auto len = Address<DOMAIN>::sockaddr_size;

            // TODO: check errors and retry if it's a transitive error
            auto ret = ::recvfrom(socket.socket(), data, size, 0,
                                  reinterpret_cast<::sockaddr *>(&sa), &len);
            if(ret < 0)
            {
                inner::emit_exception(errno);
            }
            return std::make_tuple(ret, Address<DOMAIN>{sa});
        }
    } // namespace inner

    template <Domain DOMAIN, Type TYPE, typename BYTE>
    std::size_t recv(ConnectedSocket<DOMAIN, TYPE> & socket, BYTE * data,
                     std::size_t size)
    {
        static_assert(inner::is_byte_v<BYTE>,
                      "Receive buffer must be a byte sequence");
        return inner::real_recv(socket, data, size);
    }

    template <Domain DOMAIN, Type TYPE, typename BYTE, std::size_t SIZE>
    std::size_t recv(ConnectedSocket<DOMAIN, TYPE> & socket, BYTE (&data)[SIZE])
    {
        static_assert(inner::is_byte_v<BYTE>,
                      "Receive buffer must be a byte sequence");
        return inner::real_recv(socket, data, SIZE);
    }

    template <typename SOCKET, typename BYTE, Domain DOMAIN = SOCKET::domain>
    std::tuple<std::size_t, Address<DOMAIN>>
    recv_from(SOCKET & socket, BYTE * data, std::size_t size)
    {
        static_assert(inner::is_byte_v<BYTE>,
                      "Receive buffer must be a byte sequence");
        return inner::real_recv_from(socket, data, size);
    }

    template <typename SOCKET, typename BYTE, std::size_t SIZE,
              Domain DOMAIN = SOCKET::domain>
    std::tuple<std::size_t, Address<DOMAIN>> recv_from(SOCKET & socket,
                                                       BYTE (&data)[SIZE])
    {
        static_assert(inner::is_byte_v<BYTE>,
                      "Receive buffer must be a byte sequence");
        return inner::real_recv_from(socket, data, SIZE);
    }
} // namespace checkerboard

#endif
