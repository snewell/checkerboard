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

    template <typename T>
    class Echo : public ::testing::Test
    {
    };
} // namespace

using MyTypes = ::testing::Types<std::int8_t, std::uint8_t
#if __cplusplus >= 201703L
                                 ,
                                 std::byte
#endif
                                 >;
TYPED_TEST_SUITE(Echo, MyTypes, );

TYPED_TEST(Echo, echo_inet_bind) // NOLINT
{
    std::uint8_t ip[4] = {127, 0, 0, 1};
    checkerboard::Address<checkerboard::inet> a{ip, 0};
    auto client = make_server(a);
    auto server = make_server(a);

    TypeParam const hello[] = {
        static_cast<TypeParam>('H'), static_cast<TypeParam>('e'),
        static_cast<TypeParam>('l'), static_cast<TypeParam>('l'),
        static_cast<TypeParam>('o')};
    auto sent_bytes = checkerboard::send_to(client, server.address(), hello);

    TypeParam world[5];
    checkerboard::recv_from(server, world);
    checkerboard::send_to(server, client.address(), world);

    TypeParam buffer[5];
    auto received_data = checkerboard::recv_from(client, buffer);

    ASSERT_EQ(sent_bytes, std::get<0>(received_data));
    ASSERT_TRUE(
        std::equal(std::begin(hello), std::end(hello), std::begin(buffer)));
}

TYPED_TEST(Echo, echo_inet6_bind) // NOLINT
{
    std::uint8_t ip[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    checkerboard::Address<checkerboard::inet6> a{ip, 0};
    auto client = make_server(a);
    auto server = make_server(a);

    TypeParam const hello[] = {
        static_cast<TypeParam>('H'), static_cast<TypeParam>('e'),
        static_cast<TypeParam>('l'), static_cast<TypeParam>('l'),
        static_cast<TypeParam>('o')};
    auto sent_bytes = checkerboard::send_to(client, server.address(), hello);

    TypeParam world[5];
    checkerboard::recv_from(server, world);
    checkerboard::send_to(server, client.address(), world);

    TypeParam buffer[5];
    auto received_data = checkerboard::recv_from(client, buffer);

    ASSERT_EQ(sent_bytes, std::get<0>(received_data));
    ASSERT_TRUE(
        std::equal(std::begin(hello), std::end(hello), std::begin(buffer)));
}

TYPED_TEST(Echo, echo_inet_no_bind) // NOLINT
{
    std::uint8_t ip[4] = {127, 0, 0, 1};
    checkerboard::Address<checkerboard::inet> a{ip, 0};
    auto server = make_server(a);

    checkerboard::Socket<checkerboard::inet, checkerboard::datagram> client;
    TypeParam const hello[] = {
        static_cast<TypeParam>('H'), static_cast<TypeParam>('e'),
        static_cast<TypeParam>('l'), static_cast<TypeParam>('l'),
        static_cast<TypeParam>('o')};
    auto sent_bytes = checkerboard::send_to(client, server.address(), hello);

    TypeParam world[5];
    auto received_data = checkerboard::recv_from(server, world);
    checkerboard::send_to(server, std::get<1>(received_data), world);

    TypeParam buffer[5];
    auto received_data2 = checkerboard::recv_from(client, buffer);

    ASSERT_EQ(sent_bytes, std::get<0>(received_data2));
    ASSERT_TRUE(
        std::equal(std::begin(hello), std::end(hello), std::begin(buffer)));
}

TYPED_TEST(Echo, echo_inet6_no_bind) // NOLINT
{
    std::uint8_t ip[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    checkerboard::Address<checkerboard::inet6> a{ip, 0};
    auto server = make_server(a);

    checkerboard::Socket<checkerboard::inet6, checkerboard::datagram> client;
    TypeParam const hello[] = {
        static_cast<TypeParam>('H'), static_cast<TypeParam>('e'),
        static_cast<TypeParam>('l'), static_cast<TypeParam>('l'),
        static_cast<TypeParam>('o')};
    auto sent_bytes = checkerboard::send_to(client, server.address(), hello);

    TypeParam world[5];
    auto received_data = checkerboard::recv_from(server, world);
    checkerboard::send_to(server, std::get<1>(received_data), world);

    TypeParam buffer[5];
    auto received_data2 = checkerboard::recv_from(client, buffer);

    ASSERT_EQ(sent_bytes, std::get<0>(received_data2));
    ASSERT_TRUE(
        std::equal(std::begin(hello), std::end(hello), std::begin(buffer)));
}
