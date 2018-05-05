cmake_minimum_required(VERSION 3.10)
project(cos C)

set(CMAKE_C_STANDARD 11)

add_executable(cos main.c bigf_reader.h bigf_reader.c utility.h utility.c qfs_interface.h qfs_interface.c data_block.h data_block.c stream_handler.c stream_handler.h)
