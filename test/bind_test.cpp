#include <checkerboard/socket.hpp>

#include <gtest/gtest.h>

TEST(Bind, simple_bind)
{
    checkerboard::Socket<checkerboard::inet, checkerboard::stream> s;

    std::uint8_t ip[4] = {0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet> a{
        ip, static_cast<std::uint16_t>(14000)};
    checkerboard::bind(s, a);
}

TEST(Bind, simple_bind6)
{
    checkerboard::Socket<checkerboard::inet6, checkerboard::stream> s;

    std::uint8_t ip[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet6> a{
        ip, static_cast<std::uint16_t>(14000)};
    checkerboard::bind(s, a);
}
