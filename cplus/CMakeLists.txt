cmake_minimum_required(VERSION 3.17)
project(cplus)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(cplus main.cpp)
target_link_libraries(cplus gtest_main)
