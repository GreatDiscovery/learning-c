include(FetchContent)
# For Windows: Prevent overriding the parent project's compiler/linker settings

FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
# 该参数用来开启或关闭编译器优化，优化一些函数返回的tmp对象
#add_compile_options(-Wall -fno-elide-constructors)

include(FindThreads)
include(FindProtobuf)
set(PROTO_FILES test/framework/echo.proto)
# todo 不生效
protobuf_generate_cpp(PROTO_SRC PROTO_HEADER ${PROTO_FILES})
# include PROTO_HEADER
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(
        unit_tests
        example_add.cpp
        sample1.cpp
        basic/for_test.cpp
        basic/iterator_test.cpp basic/iterator_test.h basic/std_function_test.cpp basic/std_function_test2.cpp basic/std_bind.cpp network/hello_server.cpp network/hello_client.cpp network/hello.h io/file_test.cpp io/file_test.h network/shutdown_test.cpp basic/lambda_test.cpp basic.h basic/tuple_test.cpp network/echo_server.cpp network/echo_client.cpp optimize/right_reference.cpp basic/fork_test.cpp basic/string_test.cpp basic/copy_constructor_test.cpp basic/stdin_test.cpp signal/signal_test.cpp optimize/deep_copy.cpp basic/MyString.cpp optimize/forward_test.cpp io/lseek_test.cpp collection/emplace_back_test.cpp optimize/type_traits_test.cpp optimize/decltype_test.cpp optimize/template_test.cpp collection/tuple_test.cpp network/daemon_init.cpp network/unix_local_test.cpp network/get_ifi_info.cpp network/unpifi.h basic/std_low_bound.cpp collection/vector_test.cpp collection/vector_test.h basic/macro_test.cpp concurrency/thread_test.cpp concurrency/mutex_test.cpp concurrency/atomic_test.cpp network/convert.cpp network/mutli_thread_listening.cpp algorithm/top_k_test.cpp algorithm/top_max_k_test.cpp c/memory_test.cpp basic/rand_test.cpp collection/map_test.cpp basic/keyword_test.cpp class/class_test.cpp io/fallocate_test.cpp optimize/unique_ptr_test.cpp
        optimize/move_test.cpp
        algorithm/heap_test.cpp
        algorithm/llstring_test.cpp
        c/char_test.cpp
        c/va_test.cpp
        network/socket_pari_test.cpp
        io/fcntl_test.cpp
        concurrency/pipe_test.cpp
        ipc/shm_test.cpp
        ipc/semaphore_test.cpp
        concurrency/cas_test.cpp
        concurrency/semaphore.cpp
        concurrency/leak_bucket_test.cpp
        basic/time_test.cpp
#        framework/brpc/hello_service.cpp
        framework/brpc_test.cpp
        framework/echo.pb.cc
        framework/execution_queue_test.cpp
        basic/exception_test.cpp
)

set(CMAKE_PREFIX_PATH /root/brpc/output)
# 查找 brpc 库和头文件，需要手动安装
find_path(BRPC_INCLUDE_PATH NAMES brpc/server.h)
find_library(BRPC_LIB NAMES libbrpc.a brpc)
message("BRPC_INCLUDE_DIR=${BRPC_INCLUDE_PATH}")
message("BRPC_LIB=${BRPC_LIB}")
if((NOT BRPC_INCLUDE_PATH) OR (NOT BRPC_LIB))
        message(FATAL_ERROR "Fail to find brpc")
endif()
# 设置 include 路径
include_directories(${BRPC_INCLUDE_PATH})

find_path(GFLAGS_INCLUDE_PATH gflags/gflags.h)
find_library(GFLAGS_LIBRARY NAMES gflags libgflags)
if((NOT GFLAGS_INCLUDE_PATH) OR (NOT GFLAGS_LIBRARY))
        message(FATAL_ERROR "Fail to find gflags")
endif()
include_directories(${GFLAGS_INCLUDE_PATH})

find_path(LEVELDB_INCLUDE_PATH NAMES leveldb/db.h)
find_library(LEVELDB_LIB NAMES leveldb)
if ((NOT LEVELDB_INCLUDE_PATH) OR (NOT LEVELDB_LIB))
        message(FATAL_ERROR "Fail to find leveldb")
endif()
include_directories(${LEVELDB_INCLUDE_PATH})

find_package(OpenSSL)
include_directories(${OPENSSL_INCLUDE_DIR})



# 将库文件添加到链接路径中
set(DYNAMIC_LIB
        ${CMAKE_THREAD_LIBS_INIT}
        ${GFLAGS_LIBRARY}
        ${PROTOBUF_LIBRARIES}
        ${LEVELDB_LIB}
        ${OPENSSL_CRYPTO_LIBRARY}
        ${OPENSSL_SSL_LIBRARY}
        dl
)

target_link_libraries(unit_tests
        PRIVATE
        example
        gtest_main
        ${BRPC_LIB} ${DYNAMIC_LIB}
)

# automatic discovery of unit tests
include(GoogleTest)
gtest_discover_tests(unit_tests
        PROPERTIES
        LABELS "unit"
        DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
        240
        )
