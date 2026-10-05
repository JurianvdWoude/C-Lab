cmake_minimum_required(VERSION 3.16)
project(Testing_Code LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# GOOGLETEST

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# CALCULATOR CODE

add_library(calculator src/calculator.cpp)
target_include_directories(calculator PUBLIC include)


# EXECUTABLE

add_executable(my_tests tests/calculator_test.cpp)
target_link_libraries(my_tests PRIVATE calculator GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(my_tests)

