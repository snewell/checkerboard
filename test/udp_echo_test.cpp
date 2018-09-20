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
} // namespace

TEST(Udp, echo_inet_bind) // NOLINT
{
    std::uint8_t ip[4] = {127, 0, 0, 1};
    checkerboard::Address<checkerboard::inet> a{ip, 0};
    auto client = make_server(a);
    auto server = make_server(a);

    std::int8_t const hello[] = "Hello";
    auto sent_bytes = checkerboard::send_to(client, server.address(), hello);

    std::int8_t world[6];
    checkerboard::recv_from(server, world);
    checkerboard::send_to(server, client.address(), world);

    std::int8_t buffer[6];
    auto received_data = checkerboard::recv_from(client, buffer);

    ASSERT_EQ(sent_bytes, std::get<0>(received_data));
    ASSERT_TRUE(
        std::equal(std::begin(hello), std::end(hello), std::begin(buffer)));
}

TEST(Udp, echo_inet6_bind) // NOLINT
{
    std::uint8_t ip[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    checkerboard::Address<checkerboard::inet6> a{ip, 0};
    auto client = make_server(a);
    auto server = make_server(a);

    std::uint8_t const hello[] = "Hello";
    auto sent_bytes = checkerboard::send_to(client, server.address(), hello);

    std::uint8_t world[6];
    checkerboard::recv_from(server, world);
    checkerboard::send_to(server, client.address(), world);

    std::uint8_t buffer[6];
    auto received_data = checkerboard::recv_from(client, buffer);

    ASSERT_EQ(sent_bytes, std::get<0>(received_data));
    ASSERT_TRUE(
        std::equal(std::begin(hello), std::end(hello), std::begin(buffer)));
}

TEST(Udp, echo_inet_no_bind) // NOLINT
{
    std::uint8_t ip[4] = {127, 0, 0, 1};
    checkerboard::Address<checkerboard::inet> a{ip, 0};
    auto server = make_server(a);

    checkerboard::Socket<checkerboard::inet, checkerboard::datagram> client;
    std::int8_t const hello[] = "Hello";
    auto sent_bytes = checkerboard::send_to(client, server.address(), hello);

    std::int8_t world[6];
    auto received_data = checkerboard::recv_from(server, world);
    checkerboard::send_to(server, std::get<1>(received_data), world);

    std::int8_t buffer[6];
    auto received_data2 = checkerboard::recv_from(client, buffer);

    ASSERT_EQ(sent_bytes, std::get<0>(received_data2));
    ASSERT_TRUE(
        std::equal(std::begin(hello), std::end(hello), std::begin(buffer)));
}

TEST(Udp, echo_inet6_no_bind) // NOLINT
{
    std::uint8_t ip[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    checkerboard::Address<checkerboard::inet6> a{ip, 0};
    auto server = make_server(a);

    checkerboard::Socket<checkerboard::inet6, checkerboard::datagram> client;
    std::int8_t const hello[] = "Hello";
    auto sent_bytes = checkerboard::send_to(client, server.address(), hello);

    std::int8_t world[6];
    auto received_data = checkerboard::recv_from(server, world);
    checkerboard::send_to(server, std::get<1>(received_data), world);

    std::int8_t buffer[6];
    auto received_data2 = checkerboard::recv_from(client, buffer);

    ASSERT_EQ(sent_bytes, std::get<0>(received_data2));
    ASSERT_TRUE(
        std::equal(std::begin(hello), std::end(hello), std::begin(buffer)));
}
