cmake_minimum_required(VERSION 3.18)

project(${PROJECT_NAME} LANGUAGES C CXX VERSION 1.0.0)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

set(SRC 
src/application.c
src/network.c
src/ringsender.c
)

add_executable(
  ringsend
  src/main.c
  ${SRC}
)

target_include_directories(ringsend PRIVATE include)

target_link_libraries(
  ringsend
  pthread
)

enable_testing()

add_executable(
  ringsender_test
  ${SRC}
  test/application_test.cpp
  test/ringsender_test.cpp
  test/network_test.cpp
)

target_include_directories(ringsender_test PRIVATE include)
target_link_libraries(
  ringsender_test
  gtest_main
  m
)


include(GoogleTest)
gtest_discover_tests(ringsender_test)
