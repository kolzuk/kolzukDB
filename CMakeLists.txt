cmake_minimum_required(VERSION 3.25)
project(kolzukDB)

set(CMAKE_CXX_STANDARD 20)

link_directories(lib)

add_subdirectory(lib)

enable_testing()
add_subdirectory(tests)


