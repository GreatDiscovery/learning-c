cmake_minimum_required(VERSION 3.17)
project(cplus)

set(CMAKE_CXX_STANDARD 14)

# 查找 brpc 库和头文件
find_path(BRPC_INCLUDE_DIR NAMES brpc/channel.h PATHS ${BRPC_DIR}/include)
find_library(BRPC_LIB NAMES brpc.a PATHS ${BRPC_DIR}/lib NO_DEFAULT_PATH)
message("BRPC_INCLUDE_DIR=${BRPC_INCLUDE_DIR}")
message("BRPC_LIB=${BRPC_LIB}")

# 查找其他依赖库
find_library(PROTOBUF_LIB NAMES protobuf PATHS ${BRPC_DIR}/lib)
find_library(GFLAGS_LIB NAMES gflags PATHS ${BRPC_DIR}/lib)
find_library(SSL_LIB NAMES ssl PATHS ${BRPC_DIR}/lib)
find_library(CRYPTO_LIB NAMES crypto PATHS ${BRPC_DIR}/lib)
find_library(Z_LIB NAMES z PATHS ${BRPC_DIR}/lib)
find_library(PTHREAD_LIB NAMES pthread PATHS /usr/lib /usr/local/lib)
if ((NOT PROTOBUF_LIB) OR (NOT GFLAGS_LIB) OR (NOT SSL_LIB) OR (NOT CRYPTO_LIB) OR (NOT Z_LIB) OR (NOT PTHREAD_LIB))
    message("no package")
endif()

# 设置 include 路径
include_directories(${BRPC_INCLUDE_DIR})

# 将库文件添加到链接路径中
set(LIBS ${BRPC_LIB} ${PROTOBUF_LIB} ${GFLAGS_LIB} ${SSL_LIB} ${CRYPTO_LIB} ${Z_LIB} ${PTHREAD_LIB})


add_subdirectory(src)
enable_testing()
add_subdirectory(test)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(cplus main.cpp)
target_link_libraries(cplus gtest_main ${LIBS})
