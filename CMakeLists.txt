cmake_minimum_required(VERSION 3.18)

project(${PROJECT_NAME} LANGUAGES C VERSION 1.0.0)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

set(SRC 
src/main.c)

add_executable(
  main
  ${SRC}
)

target_include_directories(main PRIVATE include)