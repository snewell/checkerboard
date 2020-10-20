#ifndef CHECKERBOARD_UNIX_HPP
#define CHECKERBOARD_UNIX_HPP 1

#include <sys/un.h>

#include <checkerboard/address.hpp>

namespace checkerboard
{
    constexpr Domain unix_socket = Domain(AF_UNIX);

    template <>
    struct sockaddr_type<checkerboard::unix_socket>
    {
        using type = ::sockaddr_un;
    };

    namespace inner
    {
        template <std::size_t PATH_LENGTH>
        ::sockaddr_un make_unix_sockaddr(char const (&path)[PATH_LENGTH])
        {
            static_assert(PATH_LENGTH <= sizeof(::sockaddr_un::sun_path),
                          "Path too long");
            ::sockaddr_un sockaddr;
            sockaddr.sun_family = AF_UNIX;
            std::strncpy(sockaddr.sun_path, path, PATH_LENGTH);
            return sockaddr;
        }

        class UnixAddress : public BaseAddress<::checkerboard::unix_socket>
        {
        public:
            using BaseAddress<::checkerboard::unix_socket>::BaseAddress;

            template <std::size_t PATH_LENGTH>
            UnixAddress(char const (&path)[PATH_LENGTH])
              : BaseAddress{make_unix_sockaddr(path)}
            {
            }
        };
    } // namespace inner

    template <>
    class Address<::checkerboard::unix_socket> : public inner::UnixAddress
    {
    public:
        using inner::UnixAddress::UnixAddress;
    };
} // namespace checkerboard

#endif
