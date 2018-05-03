cmake_minimum_required(VERSION 3.10)
project(cos C)

set(CMAKE_C_STANDARD 11)

add_executable(cos main.c bigf_reader.h bigf_reader.c utility.h utility.c)
