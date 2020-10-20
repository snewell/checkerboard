#include <checkerboard/fns.hpp>

#include <checkerboard/inet.hpp>

#include <gtest/gtest.h>

namespace
{
    template <checkerboard::Domain DOMAIN>
    checkerboard::BoundSocket<DOMAIN, checkerboard::stream>
    make_bound_socket(checkerboard::Address<DOMAIN> address)
    {
        checkerboard::Socket<DOMAIN, checkerboard::stream> s;
        return checkerboard::bind(std::move(s), address);
    }
} // namespace

TEST(Bind, simple_bind) // NOLINT
{
    std::uint8_t ip[4] = {0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet> a{ip, 0};
    auto bound_socket = make_bound_socket(a);
}

TEST(Bind, duplicate_bind) // NOLINT
{
    std::uint8_t ip[4] = {0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet> a{ip, 0};
    auto bound_socket = make_bound_socket(a);
    ASSERT_THROW(make_bound_socket(bound_socket.address()), std::runtime_error);
}

TEST(Bind, simple_bind6) // NOLINT
{
    std::uint8_t ip[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet6> a{ip, 0};
    auto bound_socket = make_bound_socket(a);
}

TEST(Bind, duplicate_bind6) // NOLINT
{
    std::uint8_t ip[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet6> a{ip, 0};
    auto bound_socket = make_bound_socket(a);
    ASSERT_THROW(make_bound_socket(bound_socket.address()), std::runtime_error);
}
