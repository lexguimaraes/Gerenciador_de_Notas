cmake_minimum_required(VERSION 3.30)
project(Testes C)

set(CMAKE_C_STANDARD 99)


find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

add_compile_options("${compiler_opts}"  ${GTK4_CFLAGS_OTHER})


file(GLOB_RECURSE sources src/*.c)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})

add_executable(Teste ${sources})
target_link_libraries(Teste ${GTK4_LIBRARIES})
