include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
        unit_tests
        example_add.cpp
        sample1.cpp
        basic/for_test.cpp
        basic/iterator_test.cpp basic/iterator_test.h basic/std_function_test.cpp basic/std_function_test2.cpp basic/std_bind.cpp network/hello_server.cpp network/hello_client.cpp network/hello.h io/file_test.cpp io/file_test.h)

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
