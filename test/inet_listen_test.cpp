#include <checkerboard/fns.hpp>

#include <checkerboard/inet.hpp>

#include <gtest/gtest.h>

namespace
{
    template <checkerboard::Domain DOMAIN>
    checkerboard::ListeningSocket<DOMAIN, checkerboard::stream>
    make_listening_socket(checkerboard::Address<DOMAIN> address)
    {
        checkerboard::Socket<DOMAIN, checkerboard::stream> s;
        auto bound_socket = checkerboard::bind(std::move(s), address);
        return checkerboard::listen(std::move(bound_socket), 1);
    }
} // namespace

TEST(Listen, simple_listen) // NOLINT
{
    std::uint8_t ip[4] = {0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet> a{ip, 0};
    auto listening_socket = make_listening_socket(a);
}

TEST(Listen, duplicate_listen) // NOLINT
{
    std::uint8_t ip[4] = {0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet> a{ip, 0};
    auto listening_socket = make_listening_socket(a);
    ASSERT_THROW(make_listening_socket(listening_socket.address()),
                 std::runtime_error);
}

TEST(Listen, simple_listen6) // NOLINT
{
    std::uint8_t ip[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet6> a{ip, 0};
    auto listening_socket = make_listening_socket(a);
}

TEST(Listen, duplicate_listen6) // NOLINT
{
    std::uint8_t ip[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet6> a{ip, 0};
    auto listening_socket = make_listening_socket(a);
    ASSERT_THROW(make_listening_socket(listening_socket.address()),
                 std::runtime_error);
}
