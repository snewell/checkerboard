cmake_minimum_required(VERSION 3.9)
cmake_policy(VERSION 3.9)

project("checkerboard"
    LANGUAGES
        CXX
    VERSION
        0.1.0
)

include(GNUInstallDirs)

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
include(CPack)

option(CHECKERBOARD_BUILD_TESTS "Build optional tests (requires GTest)" ON)

enable_testing()

if(CHECKERBOARD_BUILD_TESTS)
    find_package(GTest CONFIG REQUIRED)
    function(create_test name)
        add_executable(${name} ${ARGN})
        target_link_libraries(${name}
            GTest::gtest
            GTest::gtest_main
            checkerboard
        )
        set_target_properties(${name} PROPERTIES
            CXX_EXTENSIONS OFF
        )
        add_test(${name} ${name} "--gtest_output=xml:${CMAKE_CURRENT_BINARY_DIR}/${name}_results.xml")
    endfunction()
else()
    function(create_test name)
        #do nothing
    endfunction()
endif()

add_library(checkerboard INTERFACE)
target_include_directories(checkerboard
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(checkerboard
    INTERFACE
        cxx_std_14
)

function(add_headers)
    foreach(target
                checkerboard
    )
        foreach(header IN LISTS ARGN)
            set(full_header_path "${CMAKE_CURRENT_SOURCE_DIR}/include/checkerboard/${header}")
            list(APPEND all_headers ${full_header_path})
            target_sources(${target}
                INTERFACE
                    $<BUILD_INTERFACE:${full_header_path}>
                    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/checkerboard/${header}>
            )
        endforeach()
    endforeach()
    install(FILES ${all_headers}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/checkerboard
        COMPONENT ${devComponent}
    )
endfunction()

add_subdirectory(test)

add_headers(
    address.hpp
    domain.hpp
    fns.hpp
    sockaddr.hpp
    socket.hpp
    type.hpp
)

install(TARGETS
        checkerboard
    EXPORT CheckerboardConfig
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

export(TARGETS
        checkerboard
    NAMESPACE Checkerboard::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/CheckerboardConfig.cmake"
)
install(EXPORT
        CheckerboardConfig
    DESTINATION "${CMAKE_INSTALL_DATADIR}/Checkerboard/cmake"
    NAMESPACE Checkerboard::
)
install(FILES
        COPYING
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)
