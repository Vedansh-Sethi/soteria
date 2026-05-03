cmake_minimum_required(VERSION 3.15)
project(soteria CXX)

set(SOURCES
    main.cpp
    utils/field_element/field_element.cpp
    utils/ECP/ECP.cpp
)

find_package(Boost REQUIRED)

# Define the executable
add_executable(soteria ${SOURCES})

# Include the current directory for header lookups
target_include_directories(soteria PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(soteria PRIVATE Boost::headers)