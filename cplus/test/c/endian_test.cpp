//
// Created by 宋博文 on 2024/12/14.
//

#include "../basic.h"

TEST(endian_test, 测试机器是大端序还是小端序) {
    union w {
        int a;
        char b;
    } c;

    c.a = 1;
    // 为ture则是小端序，0x0001
    std::cout << (c.b == 1) << std::endl;
}