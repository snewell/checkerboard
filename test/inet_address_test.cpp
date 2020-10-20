#include <checkerboard/inet.hpp>

#include <gtest/gtest.h>

TEST(Address, inet_ctor) // NOLINT
{
    std::uint8_t const ip[4] = {1, 2, 3, 4};
    std::uint16_t port = 14000;
    checkerboard::Address<checkerboard::inet> a{ip, port};

    std::uint8_t out_ip[4];
    a.ip(out_ip);
    ASSERT_TRUE(
        std::equal(std::begin(out_ip), std::end(out_ip), std::begin(ip)));
    ASSERT_EQ(port, a.port());
}

TEST(Address, inet6_ctor) // NOLINT
{
    std::uint8_t const ip[16] = {1, 2,  3,  4,  5,  6,  7,  8,
                                 9, 10, 11, 12, 13, 14, 15, 16};
    std::uint16_t port = 14000;
    checkerboard::Address<checkerboard::inet6> a{ip, port};

    std::uint8_t out_ip[16];
    a.ip(out_ip);
    ASSERT_TRUE(
        std::equal(std::begin(out_ip), std::end(out_ip), std::begin(ip)));
    ASSERT_EQ(port, a.port());
}
