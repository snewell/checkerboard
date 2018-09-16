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
    } // namespace inner

    template <Domain DOMAIN>
    class Address
    {
    public:
        using sockaddr_t = sockaddr_type_t<DOMAIN>;

        static constexpr std::size_t sockaddr_size = sizeof(sockaddr_t);

        static constexpr std::size_t ip_byte_count = ip_size<DOMAIN>::value;

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

        void ip(std::uint8_t (&ip)[ip_size_v<DOMAIN>])
        {
            inner::sockaddr_ip_getter<DOMAIN>::fill_ip(_sockaddr, ip);
        }

        std::uint16_t port() const noexcept
        {
            return inner::sockaddr_port_getter<DOMAIN>::get_sockaddr_port(_sockaddr);
        }

    private:
        sockaddr_t _sockaddr;
    };
} // namespace checkerboard

#endif
