cmake_minimum_required(VERSION 3.10)
project(CollatzSolver LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(solver
  src/main.cpp
  src/core/number.cpp
)

target_include_directories(solver PRIVATE src/core)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(solver PRIVATE -Wall -Wextra)
elseif(MSVC)
  target_compile_options(solver PRIVATE /W4)
endif()