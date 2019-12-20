#include <checkerboard/fns.hpp>

#include <gtest/gtest.h>

namespace
{
    template <checkerboard::Domain DOMAIN>
    checkerboard::ListeningSocket<DOMAIN, checkerboard::stream>
    make_server(checkerboard::Address<DOMAIN> const & address)
    {
        checkerboard::Socket<DOMAIN, checkerboard::stream> s;
        auto bound_socket = checkerboard::bind(std::move(s), address);
        return checkerboard::listen(std::move(bound_socket), 1);
    }

    template <checkerboard::Domain DOMAIN>
    checkerboard::ConnectedSocket<DOMAIN, checkerboard::stream>
    connect_server(checkerboard::Address<DOMAIN> const & address)
    {
        checkerboard::Socket<DOMAIN, checkerboard::stream> s;
        return checkerboard::connect(std::move(s), address);
    }

    template <typename T>
    class Echo : public ::testing::Test
    {
    };
} // namespace

using MyTypes = ::testing::Types<std::int8_t, std::uint8_t>;
TYPED_TEST_SUITE(Echo, MyTypes, );

TYPED_TEST(Echo, echo_inet) // NOLINT
{
    std::uint8_t ip[4] = {127, 0, 0, 1};
    checkerboard::Address<checkerboard::inet> a{ip, 0};
    auto listening_socket = make_server(a);
    auto outbound_connection = connect_server(listening_socket.address());
    auto inbound_connection = checkerboard::accept(listening_socket);

    TypeParam const hello[] = "Hello";
    auto sent_bytes = checkerboard::send(outbound_connection, hello);

    TypeParam world[6];
    checkerboard::recv(std::get<0>(inbound_connection), world);
    checkerboard::send(std::get<0>(inbound_connection), world);

    TypeParam buffer[6];
    auto received_bytes = checkerboard::recv(outbound_connection, buffer);

    ASSERT_EQ(sent_bytes, received_bytes);
    ASSERT_TRUE(
        std::equal(std::begin(hello), std::end(hello), std::begin(buffer)));
}

TYPED_TEST(Echo, echo_inet6) // NOLINT
{
    std::uint8_t ip[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    checkerboard::Address<checkerboard::inet6> a{ip, 0};
    auto listening_socket = make_server(a);
    auto outbound_connection = connect_server(listening_socket.address());
    auto inbound_connection = checkerboard::accept(listening_socket);

    TypeParam const hello[] = "Hello";
    auto sent_bytes = checkerboard::send(outbound_connection, hello);

    TypeParam world[6];
    checkerboard::recv(std::get<0>(inbound_connection), world);
    checkerboard::send(std::get<0>(inbound_connection), world);

    TypeParam buffer[6];
    auto received_bytes = checkerboard::recv(outbound_connection, buffer);

    ASSERT_EQ(sent_bytes, received_bytes);
    ASSERT_TRUE(
        std::equal(std::begin(hello), std::end(hello), std::begin(buffer)));
}
