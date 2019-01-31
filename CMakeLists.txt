cmake_minimum_required(VERSION 3.5.1)
project(testing)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_compile_options(-std=c++11)

add_executable(testing ${SOURCES})