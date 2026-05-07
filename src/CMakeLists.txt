cmake_minimum_required(VERSION 3.15)
project(soteria CXX)

set(SOURCES
    main.cpp
    ECC/field_element/field_element.cpp
    ECC/ECP/ECP.cpp
    ECC/secp256k1/S256Point/S256Point.cpp
    signing/privateKey/privateKey.cpp
)

find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

# Define the executable
add_executable(soteria ${SOURCES})

# Include the current directory for header lookups
target_include_directories(soteria PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(soteria PRIVATE Boost::headers OpenSSL::Crypto OpenSSL::SSL)