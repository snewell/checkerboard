#ifndef CHECKERBOARD_SOCKADDR_HPP
#define CHECKERBOARD_SOCKADDR_HPP 1

#include <netinet/in.h>

#include <cstdint>
#include <cstring>
#include <type_traits>

#include <checkerboard/domain.hpp>

namespace checkerboard
{
    template <Domain DOMAIN>
    struct sockaddr_type;

    template <Domain DOMAIN>
    using sockaddr_type_t = typename sockaddr_type<DOMAIN>::type;

    template <Domain DOMAIN>
    struct ip_size;

    template <Domain DOMAIN>
    constexpr auto ip_size_v = ip_size<DOMAIN>::value;

    namespace inner
    {
        template <Domain DOMAIN>
        struct sockaddr_builder;

        template <Domain DOMAIN>
        struct sockaddr_port_getter;

        template <Domain DOMAIN>
        struct sockaddr_ip_getter;
    } // namespace inner

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

    namespace inner
    {
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
        struct sockaddr_port_getter<checkerboard::inet>
        {
            using sockaddr_t = sockaddr_type_t<checkerboard::inet>;

            static std::uint16_t get_sockaddr_port(sockaddr_t const & s)
            {
                return ::ntohs(s.sin_port);
            }
        };

        template <>
        struct sockaddr_ip_getter<checkerboard::inet>
        {
            using sockaddr_t = sockaddr_type_t<checkerboard::inet>;

            static void
            fill_ip(sockaddr_t const & s,
                    std::uint8_t (&ip)[ip_size_v<checkerboard::inet>])
            {
                ::memcpy(ip, &(s.sin_addr), ip_size_v<checkerboard::inet>);
            }
        };
    } // namespace inner

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

    namespace inner
    {
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

        template <>
        struct sockaddr_port_getter<checkerboard::inet6>
        {
            using sockaddr_t = sockaddr_type_t<checkerboard::inet6>;

            static std::uint16_t get_sockaddr_port(sockaddr_t const & s)
            {
                return ::ntohs(s.sin6_port);
            }
        };

        template <>
        struct sockaddr_ip_getter<checkerboard::inet6>
        {
            using sockaddr_t = sockaddr_type_t<checkerboard::inet6>;

            static void
            fill_ip(sockaddr_t const & s,
                    std::uint8_t (&ip)[ip_size_v<checkerboard::inet6>])
            {
                ::memcpy(ip, &(s.sin6_addr), ip_size_v<checkerboard::inet6>);
            }
        };
    } // namespace inner
} // namespace checkerboard

#endif
