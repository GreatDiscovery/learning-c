//
// Created by Gavin on 2022/8/1.
//

// 可调用对象
// 可参考函数指针和类成员函数指针：https://www.runoob.com/w3cnote/cpp-func-pointer.html
#include "iostream"
#include "gtest/gtest.h"

using namespace std;

void func() {
    cout << "this is a func!" << endl;
}

struct Foo {
    // 仿函数（Functor）又称为函数对象（Function Object）是一个能行使函数功能的类
    void operator()() {
        cout << "这是一个仿函数!" << endl;
    }
};

struct Bar {
    using fr_t = void (*)();

    static void func() {

    }

    operator fr_t() {
        return func;
    }
};

struct A {
    int a_;

    void mem_func() {

    }
};

// 统计数组大于10的函数
int recall_func(int *start, const int *end, bool (*pf)(int)) {
    int count = 0;
    for (int *i = start; i != end + 1; ++i) {
        count += pf(*i) ? 1 : 0;
    }
    return count;
}

bool is_greater_than_10(int num) {
    return num > 10;
}

TEST(callable_object, case1) {
//    typedef int (*fun_ptr)(int,int); // 声明一个指向同样参数、返回值的函数指针类型
    void (*func_ptr)() =&func; // 1.函数指针
    func_ptr();

    Foo foo;
    foo(); // 2. 仿函数，参考https://blog.csdn.net/k346k346/article/details/82818801

    Bar bar;
    bar(); // 3. 可转换为函数指针的类对象

    void (A::*mem_func_ptr)() = &A::mem_func; // 4. 类成员函数指针
    int A::*mem_obj_ptr = &A::a_; // 类成员指针
    A aa;
    (aa.*mem_func_ptr)();
    aa.*mem_obj_ptr = 123;
}

// 仿函数测试用例
TEST(recall_fun, case2) {
    int a[5] = {10, 100, 11, 5, 19};
    int result = recall_func(a, a + 4, is_greater_than_10);
    cout << result << endl;
}