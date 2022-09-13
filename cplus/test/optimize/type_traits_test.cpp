//
// Created by Gavin on 2022/9/6.
//

#include "../basic.h"

using namespace std;
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

TEST(traits_test, 判断类型相同) {
    cout << is_same<int, int>::value << endl;
    cout << is_same<int, unsigned int>::value << endl;
    cout << is_same<int, signed int>::value << endl;
}

int fn(int) {
    return int();
}

typedef int (&fn_ref) (int); // 函数引用
typedef int (*fn_ptr) (int); // 函数指针
// 仿函数
struct fn_class {
    int operator() (int i) {
        return i;
    }
};

TEST(traits_test, 编译期获取一个可调用对象的返回类型) {
    // decltype简介 https://www.cnblogs.com/QG-whz/p/4952980.html
    typedef std::result_of<decltype(fn) &(int)>::type A;
    typedef result_of<fn_ref(int)>::type B;
    typedef result_of<fn_ptr(int)>::type C;
    typedef result_of<fn_class(int)>::type D;

    // 可以转换boolean输出类型为true和false
    cout << boolalpha;
    cout << "typedefs of int:" << endl;

    cout << "A: " << is_same<int, A>::value << endl;
    cout << "B: " << is_same<int, B>::value << endl;
    cout << "C: " << is_same<int, C>::value << endl;
    cout << "D: " << is_same<int, D>::value << endl;
}