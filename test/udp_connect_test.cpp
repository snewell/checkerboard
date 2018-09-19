#include <checkerboard/fns.hpp>

#include <gtest/gtest.h>

namespace
{
    template <checkerboard::Domain DOMAIN>
    checkerboard::BoundSocket<DOMAIN, checkerboard::datagram>
    make_server(checkerboard::Address<DOMAIN> const & address)
    {
        checkerboard::Socket<DOMAIN, checkerboard::datagram> s;
        return checkerboard::bind(std::move(s), address);
    }

    template <checkerboard::Domain DOMAIN>
    checkerboard::ConnectedSocket<DOMAIN, checkerboard::datagram>
    connect_server(checkerboard::Address<DOMAIN> const & address)
    {
        checkerboard::Socket<DOMAIN, checkerboard::datagram> s;
        return checkerboard::connect(std::move(s), address);
    }
} // namespace

TEST(Tcp, connect_inet) // NOLINT
{
    std::uint8_t ip[4] = {127, 0, 0, 1};
    checkerboard::Address<checkerboard::inet> a{ip, 0};
    auto bound_socket = make_server(a);
    auto outbound_connection = connect_server(bound_socket.address());

    std::int8_t const hello[] = "Hello";
    auto sent_bytes = checkerboard::send(outbound_connection, hello);

    std::int8_t world[6];
    auto received_data = checkerboard::recv_from(bound_socket, world);

    ASSERT_EQ(sent_bytes, std::get<0>(received_data));
    ASSERT_TRUE(
        std::equal(std::begin(hello), std::end(hello), std::begin(world)));
}

TEST(Tcp, connect_inet6) // NOLINT
{
    std::uint8_t ip[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    checkerboard::Address<checkerboard::inet6> a{ip, 0};
    auto bound_socket = make_server(a);
    auto outbound_connection = connect_server(bound_socket.address());

    std::int8_t const hello[] = "Hello";
    auto sent_bytes = checkerboard::send(outbound_connection, hello);

    std::int8_t world[6];
    auto received_data = checkerboard::recv_from(bound_socket, world);

    ASSERT_EQ(sent_bytes, std::get<0>(received_data));
    ASSERT_TRUE(
        std::equal(std::begin(hello), std::end(hello), std::begin(world)));
}
