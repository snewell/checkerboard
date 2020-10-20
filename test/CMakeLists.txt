create_test(inet_test
    inet_address_test.cpp
    inet_bind_test.cpp
    inet_listen_test.cpp
    tcp_connect_test.cpp
    tcp_echo_test.cpp
    udp_connect_test.cpp
    udp_echo_test.cpp
)

create_test(compile_test
    address_compile_test.cpp
    fns_compile_test.cpp
    inet_compile_test.cpp
    socket_compile_test.cpp
    type_compile_test.cpp
    unix_compile_test.cpp
)

create_test(unix_test
    unix_echo_stream_test.cpp
)
