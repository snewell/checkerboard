#include <checkerboard/socket.hpp>

#include <gtest/gtest.h>

TEST(Listen, simple_listen)
{
    checkerboard::Socket<checkerboard::inet, checkerboard::stream> s;

    std::uint8_t ip[4] = {0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet> a{ip, static_cast<std::uint16_t>(14000)};
    checkerboard::bind(s, a);
    checkerboard::listen(s, 10);
    sleep(10);
}

TEST(Listen, simple_listen6)
{
    checkerboard::Socket<checkerboard::inet6, checkerboard::stream> s;

    std::uint8_t ip[16] = {0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet6> a{ip, static_cast<std::uint16_t>(14000)};
    checkerboard::bind(s, a);
    checkerboard::listen(s, 10);
    sleep(10);
}
