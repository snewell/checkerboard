#ifndef CHECKERBOARD_SOCKET_HPP
#define CHECKERBOARD_SOCKET_HPP 1

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <type_traits>

#include <checkerboard/address.hpp>
#include <checkerboard/domain.hpp>
#include <checkerboard/type.hpp>

namespace checkerboard
{
    template <Domain DOMAIN, Type TYPE>
    class Socket
    {
    public:
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
    class BoundSocket : public Socket<DOMAIN, TYPE>
    {
    public:
        explicit BoundSocket(Socket<DOMAIN, TYPE> &&s)
          : Socket<DOMAIN, TYPE>{std::move(s)}
        {
            // TODO: make sure s wasn't invalid
        }
    };

    template <Domain DOMAIN, Type TYPE>
    class ListeningSocket : public BoundSocket<DOMAIN, TYPE>
    {
    public:
        explicit ListeningSocket(Socket<DOMAIN, TYPE> &&s)
          : BoundSocket<DOMAIN, TYPE>{std::move(s)}
        {
            // TODO: make sure s wasn't invalid
        }
    };
} // namespace checkerboard

#endif
