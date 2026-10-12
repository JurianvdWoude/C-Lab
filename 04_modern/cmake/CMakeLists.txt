cmake_minimum_required(VERSION 3.10)

project(cmake_practice)

add_library(mylibrary library.cpp library.h)
add_executable(main main.cpp)
target_link_libraries(main PRIVATE mylibrary)

