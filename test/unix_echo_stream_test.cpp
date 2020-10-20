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
    class UnixStreamEcho : public ::testing::Test
    {
    };
} // namespace

using MyTypes = ::testing::Types<std::int8_t, std::uint8_t
#if __cplusplus >= 201703L
                                 ,
                                 std::byte
#endif
                                 >;
TYPED_TEST_SUITE(UnixStreamEcho, MyTypes, );

TYPED_TEST(UnixStreamEcho, echo_inet) // NOLINT
{
    char const path[] = "/tmp/checkboard.socket";
    ::unlink(path);
    checkerboard::Address<checkerboard::unix_socket> a{path};
    auto listening_socket = make_server(a);
    auto outbound_connection = connect_server(listening_socket.address());
    auto inbound_connection = checkerboard::accept(listening_socket);

    TypeParam const hello[] = {
        static_cast<TypeParam>('H'), static_cast<TypeParam>('e'),
        static_cast<TypeParam>('l'), static_cast<TypeParam>('l'),
        static_cast<TypeParam>('o')};
    auto sent_bytes = checkerboard::send(outbound_connection, hello);

    TypeParam world[5];
    checkerboard::recv(std::get<0>(inbound_connection), world);
    checkerboard::send(std::get<0>(inbound_connection), world);

    TypeParam buffer[5];
    auto received_bytes = checkerboard::recv(outbound_connection, buffer);

    ASSERT_EQ(sent_bytes, received_bytes);
    ASSERT_TRUE(
        std::equal(std::begin(hello), std::end(hello), std::begin(buffer)));
}

TYPED_TEST(UnixStreamEcho, echo_inet6) // NOLINT
{
    char const path[] = "/tmp/checkboard.socket";
    ::unlink(path);
    checkerboard::Address<checkerboard::unix_socket> a{path};
    auto listening_socket = make_server(a);
    auto outbound_connection = connect_server(listening_socket.address());
    auto inbound_connection = checkerboard::accept(listening_socket);

    TypeParam const hello[] = {
        static_cast<TypeParam>('H'), static_cast<TypeParam>('e'),
        static_cast<TypeParam>('l'), static_cast<TypeParam>('l'),
        static_cast<TypeParam>('o')};
    auto sent_bytes = checkerboard::send(outbound_connection, hello);

    TypeParam world[5];
    checkerboard::recv(std::get<0>(inbound_connection), world);
    checkerboard::send(std::get<0>(inbound_connection), world);

    TypeParam buffer[5];
    auto received_bytes = checkerboard::recv(outbound_connection, buffer);

    ASSERT_EQ(sent_bytes, received_bytes);
    ASSERT_TRUE(
        std::equal(std::begin(hello), std::end(hello), std::begin(buffer)));
}
