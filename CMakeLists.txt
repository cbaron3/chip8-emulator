cmake_minimum_required(VERSION 3.7)
project(main)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} include)

file(GLOB SOURCES "src/*.cpp")

add_compile_options(-std=c++17)

add_executable(main ${SOURCES})

target_link_libraries(main ${SDL2_LIBRARIES})