#include <checkerboard/fns.hpp>

#include <gtest/gtest.h>

TEST(Listen, simple_listen) // NOLINT
{
    checkerboard::Socket<checkerboard::inet, checkerboard::stream> s;

    std::uint8_t ip[4] = {0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet> a{
        ip, static_cast<std::uint16_t>(14000)};
    auto bound_socket = checkerboard::bind(std::move(s), a);
    auto listening_socket =
        checkerboard::listen(std::move(std::move(bound_socket)), 10);
    sleep(10);
}

TEST(Listen, simple_listen6) // NOLINT
{
    checkerboard::Socket<checkerboard::inet6, checkerboard::stream> s;

    std::uint8_t ip[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    checkerboard::Address<checkerboard::inet6> a{
        ip, static_cast<std::uint16_t>(14000)};
    auto bound_socket = checkerboard::bind(std::move(s), a);
    auto listening_socket = checkerboard::listen(std::move(bound_socket), 10);
    sleep(10);
}
