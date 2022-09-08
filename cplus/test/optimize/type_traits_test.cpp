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

struct GetLifeSize2 : std::integral_constant<int, 1> {
    // 继承了integral_constant
};

TEST(traits_test, 获取编译期常量值) {
    std::cout << GetLeftSize::value << std::endl;
    std::cout << GetRightSize::value << std::endl;
    std::cout << GetLifeSize2::value << std::endl;
}

TEST(traits_test, 判断类型) {
    std::cout << "is_const:" << std::endl;
    std::cout << "int: " << std::is_const<int>::value << std::endl;
    std::cout << "const int: " << std::is_const<const int>::value << std::endl;
    std::cout << "const int&: " << std::is_const<const int&>::value << std::endl;
    std::cout << "const int*: " << std::is_const<const int*>::value << std::endl;
    std::cout << "int* const: " << std::is_const<int* const>::value << std::endl;
}