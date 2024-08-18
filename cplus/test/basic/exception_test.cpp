//
// Created by 宋博文 on 2024/8/18.
//

#include "../basic.h"

void divide(int a, int b) {
    if (b == 0) {
        throw std::runtime_error("Division by zero error"); // 抛出异常
    }
    std::cout << "Result: " << a / b << std::endl;
}

TEST(exception_test, Cplus_exception) {
    try {
        divide(10, 2); // 正常情况
        divide(10, 0); // 将抛出异常
    } catch (const std::runtime_error &e) { // 捕获异常
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }

    std::cout << "Program continues..." << std::endl;
}