#ifndef CHECKERBOARD_SOCKET_HPP
#define CHECKERBOARD_SOCKET_HPP 1

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <type_traits>
#include <utility>

#include <checkerboard/address.hpp>
#include <checkerboard/type.hpp>

namespace checkerboard
{
    template <Domain DOMAIN, Type TYPE>
    class Socket
    {
    public:
        static constexpr Domain domain = DOMAIN;

        static constexpr Type type = TYPE;

        Socket()
          : _socket{
                ::socket(static_cast<std::underlying_type_t<Domain>>(DOMAIN),
                         static_cast<std::underlying_type_t<Type>>(TYPE), 0)}
        {
            if(_socket == invalid_socket)
            {
                // TODO: throw exception
            }
        }

        Socket(int socket)
          : _socket{socket}
        {
            // TODO: verify socket is valid
        }

        Socket(Socket && s) noexcept
          : _socket{s._socket}
        {
            s._socket = invalid_socket;
        }

        Socket & operator=(Socket && s) noexcept
        {
            std::swap(_socket, s._socket);
            return *this;
        }

        ~Socket() noexcept
        {
            if(_socket != invalid_socket)
            {
                ::close(_socket);
            }
        }

        int socket() const noexcept
        {
            return _socket;
        }

        Socket(Socket const & s) = delete;

        Socket & operator=(Socket const & s) = delete;

    private:
        constexpr static int invalid_socket = -1;

        int _socket;
    };

    template <Domain DOMAIN, Type TYPE>
    class BoundSocket : private Socket<DOMAIN, TYPE>
    {
    public:
        explicit BoundSocket(Socket<DOMAIN, TYPE> && s)
          : Socket<DOMAIN, TYPE>{std::move(s)}
        {
            // TODO: make sure s wasn't invalid
        }

        Address<DOMAIN> address() const noexcept
        {
            typename Address<DOMAIN>::sockaddr_type sa;
            auto len = Address<DOMAIN>::sockaddr_size;
            ::getsockname(socket(), reinterpret_cast<::sockaddr *>(&sa), &len);
            return Address<DOMAIN>{sa};
        }

        using Socket<DOMAIN, TYPE>::domain;
        using Socket<DOMAIN, TYPE>::type;
        using Socket<DOMAIN, TYPE>::socket;
    };

    template <Domain DOMAIN, Type TYPE>
    class ConnectedSocket : private BoundSocket<DOMAIN, TYPE>
    {
    public:
        explicit ConnectedSocket(Socket<DOMAIN, TYPE> && s)
          : BoundSocket<DOMAIN, TYPE>{std::move(s)}
        {
            // TODO: make sure s wasn't invalid
        }

        using BoundSocket<DOMAIN, TYPE>::domain;
        using BoundSocket<DOMAIN, TYPE>::type;
        using BoundSocket<DOMAIN, TYPE>::socket;
        using BoundSocket<DOMAIN, TYPE>::address;
    };

    template <Domain DOMAIN, Type TYPE>
    class ListeningSocket : private BoundSocket<DOMAIN, TYPE>
    {
        static_assert(accepts_connections_v<TYPE>, "Invalid socket type");

    public:
        explicit ListeningSocket(BoundSocket<DOMAIN, TYPE> && s)
          : BoundSocket<DOMAIN, TYPE>{std::move(s)}
        {
            // TODO: make sure s wasn't invalid
        }

        using BoundSocket<DOMAIN, TYPE>::socket;
        using BoundSocket<DOMAIN, TYPE>::address;
    };

    template <typename SOCKET>
    struct connectionless_send;

    template <typename SOCKET>
    static constexpr auto connectionless_send_v =
        connectionless_send<SOCKET>::value;

    template <Domain DOMAIN>
    struct connectionless_send<Socket<DOMAIN, checkerboard::datagram>>
    {
        static constexpr bool value = true;
    };

    template <Domain DOMAIN>
    struct connectionless_send<BoundSocket<DOMAIN, checkerboard::datagram>>
    {
        static constexpr bool value = true;
    };
} // namespace checkerboard

#endif
