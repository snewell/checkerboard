#ifndef CHECKERBOARD_ADDRESS_HPP
#define CHECKERBOARD_ADDRESS_HPP 1

#include <sys/socket.h>

#include <cstring>

namespace checkerboard
{
    enum class Domain
    {
    };

    template <Domain DOMAIN>
    struct sockaddr_type;

    template <Domain DOMAIN>
    using sockaddr_type_t = typename sockaddr_type<DOMAIN>::type;

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
    } // namespace inner

    template <Domain DOMAIN>
    class Address;
} // namespace checkerboard

#endif
