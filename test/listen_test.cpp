#include <checkerboard/socket.hpp>

#include <gtest/gtest.h>

TEST(Listen, simple_listen)
{
    checkerboard::Socket<checkerboard::Domain::inet, checkerboard::Type::stream> s;

    std::uint8_t ip[4] = {0, 0, 0, 0};
    checkerboard::Address<checkerboard::Domain::inet> a{ip, static_cast<std::uint16_t>(14000)};
    checkerboard::bind(s, a);
    checkerboard::listen(s, 10);
    sleep(10);
}
