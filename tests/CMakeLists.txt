cmake_minimum_required(VERSION 3.0.0)
project(embedded-crypto-ticker-tests)
set(CMAKE_CXX_STANDARD 17)

enable_testing()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/doctest
 )

add_library(doctest doctest/doctest.cpp)

add_subdirectory(esp32/main/containers)
add_subdirectory(esp32/main/lib/)

add_executable(test_example example/test_example.cpp)
target_link_libraries(test_example doctest)

add_test(NAME test_example COMMAND test_example)