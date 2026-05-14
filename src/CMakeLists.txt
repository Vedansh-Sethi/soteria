cmake_minimum_required(VERSION 3.15)
project(soteria CXX)

include(FetchContent)

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.5
)

FetchContent_MakeAvailable(cpr)

file(GLOB MATH_SOURCES CONFIGURE_DEPENDS "math/*.cpp")
file(GLOB CRYPTO_SOURCES CONFIGURE_DEPENDS "crypto/*.cpp")
file(GLOB UTILS_SOURCES CONFIGURE_DEPENDS "utils/*.cpp")
file(GLOB CORE_SOURCES CONFIGURE_DEPENDS "core/*.cpp")

set(SOURCES
    main.cpp
    ${MATH_SOURCES}
    ${CRYPTO_SOURCES}
    ${UTILS_SOURCES}
    ${CORE_SOURCES}
)

find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

# Define the executable
add_executable(soteria ${SOURCES})

# Include the current directory for header lookups
target_include_directories(soteria PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(soteria PRIVATE Boost::headers OpenSSL::Crypto OpenSSL::SSL cpr::cpr)