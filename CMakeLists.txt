cmake_minimum_required(VERSION 3.13)

enable_language(CXX)
project(RPL4)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

file(GLOB SRCS src/*.cpp)
add_library(RPL4 ${SRCS})
target_include_directories(RPL4 PUBLIC ./include/)