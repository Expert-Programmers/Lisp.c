cmake_minimum_required(VERSION 3.7)
project(simpleRPEL)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c src/base.c src/base.h src/cons.c src/cons.h src/atom.c src/atom.h src/integer.c src/integer.h src/cfunc.c src/cfunc.h)
add_executable(simpleRPEL ${SOURCE_FILES})