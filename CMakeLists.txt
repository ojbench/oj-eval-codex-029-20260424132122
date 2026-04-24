cmake_minimum_required(VERSION 3.10)
project(problem_029_malloc C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(code
    src/main.c
    src/allocator.c
)

target_include_directories(code PRIVATE src)

