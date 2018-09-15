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
    template<Domain DOMAIN>
    class Address;

    namespace inner
    {
        template <typename SOCKADDR, std::size_t IP_BYTES>
        SOCKADDR make_sockaddr(Domain domain, std::uint8_t const (&ip)[IP_BYTES], std::uint16_t port)
        {
            SOCKADDR sockaddr;
            ::memset(&sockaddr, 0, sizeof(SOCKADDR));
            ::memcpy(&(sockaddr.sin_addr), ip, IP_BYTES);
            sockaddr.sin_family = static_cast<std::underlying_type_t<Domain>>(domain);
            sockaddr.sin_port = ::htons(port);

            return sockaddr;
        }

        template <Domain DOMAIN, typename SOCKADDR, std::size_t IP_BYTES>
        class SockaddrAddress
        {
        public:
            using sockaddr_t = SOCKADDR;

            static constexpr std::size_t sockaddr_size = sizeof(SOCKADDR);

            SockaddrAddress(SOCKADDR sockaddr)
            {
                ::memcpy(&_sockaddr, &sockaddr, sockaddr_size);
            }

            SockaddrAddress(std::uint8_t const (&ip)[IP_BYTES], std::uint16_t port)
              : SockaddrAddress{make_sockaddr<SOCKADDR, IP_BYTES>(DOMAIN, ip, port)}
            {
            }

            ::sockaddr const* sockaddr() const noexcept
            {
                return reinterpret_cast<::sockaddr const*>(&_sockaddr);
            }

        private:
            SOCKADDR _sockaddr;
        };
    }

    template <>
    class Address<Domain::inet> : public inner::SockaddrAddress<Domain::inet, ::sockaddr_in, 4>
    {
    public:
        template <typename ...Ts>
        Address(Ts && ...ts)
          : inner::SockaddrAddress<Domain::inet, ::sockaddr_in, 4>{std::forward<Ts>(ts)...}
        {
        }
    };
}

#endif
