cmake_minimum_required(VERSION 3.17)
project(cplus)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制要求 C++20 标准

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_subdirectory(src)
enable_testing()
add_subdirectory(test)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(cplus main.cpp)
target_link_libraries(cplus gtest_main)
