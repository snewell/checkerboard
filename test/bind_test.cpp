#include <checkerboard/fns.hpp>

#include <gtest/gtest.h>

TEST(Bind, simple_bind) // NOLINT
{
    checkerboard::Socket<checkerboard::inet, checkerboard::stream> s;

    std::uint8_t ip[4] = {0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet> a{
        ip, static_cast<std::uint16_t>(14000)};
    auto bound_socket = checkerboard::bind(std::move(s), a);
}

TEST(Bind, simple_bind6) // NOLINT
{
    checkerboard::Socket<checkerboard::inet6, checkerboard::stream> s;

    std::uint8_t ip[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet6> a{
        ip, static_cast<std::uint16_t>(14000)};
    auto bound_socket = checkerboard::bind(std::move(s), a);
}
