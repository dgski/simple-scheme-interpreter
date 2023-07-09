cmake_minimum_required(VERSION 3.13.2)

set(CMAKE_CXX_COMPILER "g++-13")
project(simple-scheme-interpreter)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES expression.cpp parser.cpp interpreter.cpp repl.cpp)
add_executable(scheme ${SOURCES})