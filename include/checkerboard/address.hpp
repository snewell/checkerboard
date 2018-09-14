#ifndef CHECKERBOARD_ADDRESS_HPP
#define CHECKERBOARD_ADDRESS_HPP 1

#include <netinet/in.h>

#include <cstring>

#include <checkerboard/domain.hpp>

namespace checkerboard
{
    template<Domain DOMAIN>
    class Address;

    template <>
    class Address<Domain::inet>
    {
    public:
        Address(::sockaddr_in sockaddr)
        {
            ::memcpy(&_sockaddr, &sockaddr, sizeof(::sockaddr_in));
        }

    private:
        ::sockaddr_in _sockaddr;
    };
}

#endif
