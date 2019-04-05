cmake_minimum_required(VERSION 3.5.1)

add_compile_options(-std=c++17)

# Get GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link run tests
add_executable(tests tests.cpp)
target_link_libraries(tests gmock pthread)