cmake_minimum_required(VERSION 3.25)
project(throttr-protocol LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g --coverage -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

option(BUILD_PROTOCOL_TESTS "Build PROTOCOL tests" OFF)

add_library(throttr-protocol INTERFACE)
target_include_directories(throttr-protocol INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(throttr-protocol INTERFACE cxx_std_23)

include(CMakePackageConfigHelpers)

install(TARGETS throttr-protocol
        EXPORT throttr-protocolTargets
)

install(EXPORT throttr-protocolTargets
        FILE throttr-protocolTargets.cmake
        NAMESPACE throttr::
        DESTINATION lib/cmake/throttr-protocol
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/throttr-protocolConfigVersion.cmake"
        VERSION 7.2.0
        COMPATIBILITY SameMajorVersion
)

install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/throttr-protocolConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/throttr-protocolConfigVersion.cmake"
        DESTINATION lib/cmake/throttr-protocol
)

if (BUILD_PROTOCOL_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc)

    add_executable(throttr-protocol-tests ${TEST_FILES})

    target_compile_options(throttr-protocol-tests PRIVATE --coverage -O0 -g -fprofile-arcs -ftest-coverage)
    target_link_options(throttr-protocol-tests PRIVATE --coverage)

    target_link_libraries(throttr-protocol-tests
            GTest::gtest_main
            throttr-protocol
    )

    include(GoogleTest)
    gtest_discover_tests(throttr-protocol-tests)
endif()

add_library(throttr::protocol ALIAS throttr-protocol)