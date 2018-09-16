create_test(address_test
    address_test.cpp
)

create_test(compile_test
    address_compile_test.cpp
    domain_compile_test.cpp
    fns_compile_test.cpp
    sockaddr_compile_test.cpp
    socket_compile_test.cpp
    type_compile_test.cpp
)

create_test(socket_test
    bind_test.cpp
    listen_test.cpp
)

create_test(tcp_test
    tcp_connect_test.cpp
)
