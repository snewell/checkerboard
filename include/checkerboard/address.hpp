#ifndef CHECKERBOARD_ADDRESS_HPP
#define CHECKERBOARD_ADDRESS_HPP 1

#include <cstdint>
#include <cstring>
#include <type_traits>
#include <utility>

#include <checkerboard/domain.hpp>
#include <checkerboard/sockaddr.hpp>

namespace checkerboard
{
    namespace inner
    {
        template <Domain DOMAIN>
        class BaseAddress
        {
        public:
            using sockaddr_type = sockaddr_type_t<DOMAIN>;

            static constexpr ::socklen_t sockaddr_size = sizeof(sockaddr_type);

            BaseAddress(sockaddr_type sockaddr)
            {
                ::memcpy(&_sockaddr, &sockaddr, sockaddr_size);
            }

            ::sockaddr const * sockaddr() const noexcept
            {
                return reinterpret_cast<::sockaddr const *>(&_sockaddr);
            }

        protected:
            sockaddr_type _sockaddr;
        };

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

        class UnixAddress : public BaseAddress<::checkerboard::unix_socket>
        {
        public:
            using BaseAddress<::checkerboard::unix_socket>::BaseAddress;

            template <std::size_t PATH_LENGTH>
            UnixAddress(char const (&path)[PATH_LENGTH])
              : BaseAddress{sockaddr_builder<
                    ::checkerboard::unix_socket>::make_sockaddr(path)}
            {
            }
        };
    } // namespace inner

    template <Domain DOMAIN>
    class Address;

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

    template <>
    class Address<::checkerboard::unix_socket> : public inner::UnixAddress
    {
    public:
        using inner::UnixAddress::UnixAddress;
    };
} // namespace checkerboard

#endif
