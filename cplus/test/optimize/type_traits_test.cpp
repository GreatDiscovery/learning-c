//
// Created by Gavin on 2022/9/6.
//

#include "../basic.h"

struct GetLeftSize {
    static const int value = 1;
};

struct GetRightSize {
    enum {
        value = 2
    };
};

TEST(traits_test, 获取编译期常量值) {
    std::cout << GetLeftSize::value << std::endl;
    std::cout << GetRightSize::value << std::endl;
}