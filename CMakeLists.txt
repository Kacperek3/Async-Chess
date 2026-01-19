cmake_minimum_required(VERSION 3.23)
project(Chess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

file(GLOB_RECURSE SOURCES Game/*.cpp)
add_executable(Chess ${SOURCES})


target_include_directories(Chess PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/include/
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(Chess PRIVATE sfml-graphics sfml-window sfml-system sfml-audio sfml-network)

