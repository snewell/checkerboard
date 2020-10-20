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
    } // namespace inner

    template <Domain DOMAIN,
              std::enable_if_t<inner::is_inet_domain_v<DOMAIN>, int> = 0>
    class Address : public inner::BaseAddress<DOMAIN>
    {
    public:
        static constexpr std::size_t ip_byte_count = ip_size<DOMAIN>::value;
        using inner::BaseAddress<DOMAIN>::BaseAddress;
        Address(std::uint8_t const (&ip)[ip_byte_count], std::uint16_t port)
          : Address{inner::sockaddr_builder<DOMAIN>::make_sockaddr(ip, port)}
        {
        }

        void ip(std::uint8_t (&ip)[ip_size_v<DOMAIN>])
        {
            inner::sockaddr_ip_getter<DOMAIN>::fill_ip(
                inner::BaseAddress<DOMAIN>::_sockaddr, ip);
        }

        std::uint16_t port() const noexcept
        {
            return inner::sockaddr_port_getter<DOMAIN>::get_sockaddr_port(
                inner::BaseAddress<DOMAIN>::_sockaddr);
        }
    };
} // namespace checkerboard

#endif
