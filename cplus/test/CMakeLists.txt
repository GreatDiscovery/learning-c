include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/refs/tags/release-1.8.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 该参数用来开启或关闭编译器优化，优化一些函数返回的tmp对象
#add_compile_options(-Wall -fno-elide-constructors)

add_executable(
        unit_tests
        example_add.cpp
        sample1.cpp
        basic/for_test.cpp
        basic/iterator_test.cpp basic/iterator_test.h basic/std_function_test.cpp basic/std_function_test2.cpp basic/std_bind.cpp network/hello_server.cpp network/hello_client.cpp network/hello.h io/file_test.cpp io/file_test.h network/shutdown_test.cpp basic/lambda_test.cpp basic.h basic/tuple_test.cpp network/echo_server.cpp network/echo_client.cpp optimize/right_reference.cpp basic/fork_test.cpp basic/string_test.cpp basic/copy_constructor_test.cpp basic/stdin_test.cpp signal/signal_test.cpp optimize/deep_copy.cpp basic/MyString.cpp optimize/forward_test.cpp io/lseek_test.cpp collection/emplace_back_test.cpp optimize/type_traits_test.cpp optimize/decltype_test.cpp optimize/template_test.cpp collection/tuple_test.cpp network/daemon_init.cpp network/unix_local_test.cpp network/get_ifi_info.cpp network/unpifi.h basic/std_low_bound.cpp collection/vector_test.cpp collection/vector_test.h basic/macro_test.cpp concurrency/thread_test.cpp concurrency/mutex_test.cpp concurrency/atomic_test.cpp network/convert.cpp network/mutli_thread_listening.cpp algorithm/top_k_test.cpp algorithm/top_max_k_test.cpp basic/rand_test.cpp)

target_link_libraries(unit_tests
        PRIVATE
        example
        gtest_main
        )

# automatic discovery of unit tests
include(GoogleTest)
gtest_discover_tests(unit_tests
        PROPERTIES
        LABELS "unit"
        DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
        240
        )
