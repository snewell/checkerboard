#ifndef CHECKERBOARD_ADDRESS_HPP
#define CHECKERBOARD_ADDRESS_HPP 1

#include <netinet/in.h>

#include <cstdint>
#include <cstring>
#include <type_traits>
#include <utility>

#include <checkerboard/domain.hpp>

namespace checkerboard
{
    namespace inner
    {
        template <Domain DOMAIN>
        struct sockaddr_type;

        template <Domain DOMAIN>
        using sockaddr_type_t = typename sockaddr_type<DOMAIN>::type;

        template <Domain DOMAIN>
        struct ip_size;

        template <Domain DOMAIN>
        constexpr auto ip_size_v = ip_size<DOMAIN>::value;

        template <Domain DOMAIN>
        struct sockaddr_builder;

        template <>
        struct sockaddr_type<checkerboard::inet>
        {
            using type = ::sockaddr_in;
        };

        template <>
        struct ip_size<checkerboard::inet>
        {
            static constexpr std::size_t value = 4;
        };

        template <>
        struct sockaddr_builder<checkerboard::inet>
        {
            using sockaddr_t = sockaddr_type_t<checkerboard::inet>;

            static constexpr auto ip_byte_count = ip_size_v<checkerboard::inet>;

            static sockaddr_t
            make_sockaddr(std::uint8_t const (&ip)[ip_byte_count],
                          std::uint16_t port)
            {
                sockaddr_t sockaddr;
                ::memset(&sockaddr, 0, sizeof(sockaddr_t));
                ::memcpy(&(sockaddr.sin_addr), ip, ip_byte_count);
                sockaddr.sin_family =
                    static_cast<std::underlying_type_t<Domain>>(
                        checkerboard::inet);
                sockaddr.sin_port = ::htons(port);

                return sockaddr;
            }
        };

        template <>
        struct sockaddr_type<checkerboard::inet6>
        {
            using type = ::sockaddr_in6;
        };

        template <>
        struct ip_size<checkerboard::inet6>
        {
            static constexpr std::size_t value = 16;
        };

        template <>
        struct sockaddr_builder<checkerboard::inet6>
        {
            using sockaddr_t = sockaddr_type_t<checkerboard::inet6>;

            static constexpr auto ip_byte_count =
                ip_size_v<checkerboard::inet6>;

            static sockaddr_t
            make_sockaddr(std::uint8_t const (&ip)[ip_byte_count],
                          std::uint16_t port)
            {
                sockaddr_t sockaddr;
                ::memset(&sockaddr, 0, sizeof(sockaddr_t));
                ::memcpy(&(sockaddr.sin6_addr), ip, ip_byte_count);
                sockaddr.sin6_family =
                    static_cast<std::underlying_type_t<Domain>>(
                        checkerboard::inet6);
                sockaddr.sin6_port = ::htons(port);

                return sockaddr;
            }
        };
    } // namespace inner

    template <Domain DOMAIN>
    class Address
    {
    public:
        using sockaddr_t = inner::sockaddr_type_t<DOMAIN>;

        static constexpr std::size_t sockaddr_size = sizeof(sockaddr_t);

        static constexpr std::size_t ip_byte_count =
            inner::ip_size<DOMAIN>::value;

        Address(sockaddr_t sockaddr)
        {
            ::memcpy(&_sockaddr, &sockaddr, sockaddr_size);
        }

        Address(std::uint8_t const (&ip)[ip_byte_count], std::uint16_t port)
          : Address{inner::sockaddr_builder<DOMAIN>::make_sockaddr(ip, port)}
        {
        }

        ::sockaddr const * sockaddr() const noexcept
        {
            return reinterpret_cast<::sockaddr const *>(&_sockaddr);
        }

    private:
        sockaddr_t _sockaddr;
    };
} // namespace checkerboard

#endif
