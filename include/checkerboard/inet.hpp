#ifndef CHECKERBOARD_INET_HPP
#define CHECKERBOARD_INET_HPP 1

#include <netinet/in.h>

#include <cstdint>
#include <type_traits>

#include <checkerboard/address.hpp>

namespace checkerboard
{
    constexpr Domain inet = Domain(AF_INET);
    constexpr Domain inet6 = Domain(AF_INET6);

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

        template <Domain DOMAIN>
        struct is_inet_domain
        {
            static constexpr bool value = false;
        };

        template <Domain DOMAIN>
#if __cplusplus >= 201703L
        inline
#endif
            constexpr auto is_inet_domain_v = is_inet_domain<DOMAIN>::value;

        template <>
        struct is_inet_domain<inet>
        {
            static constexpr bool value = true;
        };
        template <>
        struct is_inet_domain<inet6>
        {
            static constexpr bool value = true;
        };
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

    namespace inner
    {

        template <Domain DOMAIN>
        class InetAddress : public BaseAddress<DOMAIN>
        {
        public:
            static_assert(is_inet_domain_v<DOMAIN>,
                          "Cannot use non-inet domain with InetAddress");

            static constexpr std::size_t ip_byte_count = ip_size<DOMAIN>::value;
            using BaseAddress<DOMAIN>::BaseAddress;
            InetAddress(std::uint8_t const (&ip)[ip_byte_count],
                        std::uint16_t port)
              : InetAddress{
                    inner::sockaddr_builder<DOMAIN>::make_sockaddr(ip, port)}
            {
            }

            void ip(std::uint8_t (&ip)[ip_size_v<DOMAIN>])
            {
                inner::sockaddr_ip_getter<DOMAIN>::fill_ip(
                    BaseAddress<DOMAIN>::_sockaddr, ip);
            }

            std::uint16_t port() const noexcept
            {
                return inner::sockaddr_port_getter<DOMAIN>::get_sockaddr_port(
                    BaseAddress<DOMAIN>::_sockaddr);
            }
        };
    } // namespace inner

    template <>
    class Address<::checkerboard::inet>
      : public inner::InetAddress<::checkerboard::inet>
    {
    public:
        using inner::InetAddress<::checkerboard::inet>::InetAddress;
    };

    template <>
    class Address<::checkerboard::inet6>
      : public inner::InetAddress<::checkerboard::inet6>
    {
    public:
        using inner::InetAddress<::checkerboard::inet6>::InetAddress;
    };
} // namespace checkerboard

#endif
