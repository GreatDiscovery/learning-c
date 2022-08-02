//
// Created by Gavin on 2022/8/3.
//
// 可调用对象包装器

#include "iostream"
#include "functional"
#include "gtest/gtest.h"

using namespace std;

void func1() {
    cout << __FUNCTION__ << endl;
}

class Foo {
public:
    static int foo_func(int a) {
        cout << __FUNCTION__ << "(" << a << ") ->: ";
        return a;
    }
};

class Bar {
public:
    int operator()(int a) {
        cout << __FUNCTION__ << "(" << a << ") ->: ";
        return a;
    }
};

// function作为回调函数
class A {
    function<void()> callback_;

public:
    A(const function<void()> &f) : callback_(f) {}

    void notify() {
        callback_();
    }
};

class Foo2 {
public:
    void operator()() {
        cout << __FUNCTION__ << endl;
    }
};

// function作为入参
void call_when_even(int x, const function<void(int)> &f) {
    if (!(x & 1)) {
        f(x);
    }
}

void output(int x) {
    cout << x << endl;
}
TEST(std_function, case1) {
    // 普通函数
    function<void()> fr1 = func1;
    fr1();

    // 类的静态成员函数
    function<int(int)> fr2 = Foo::foo_func;
    cout << fr2(123) << endl;

    // 绑定仿函数
    Bar bar;
    fr2 = bar;
    cout << fr2(123) << endl;
}

TEST(std_function, case2) {
    Foo2 foo2;
    // std::function作为入参
    A aa(foo2);
    aa.notify();
}

TEST(std_function, case3) {
    for (int i = 0; i < 10; ++i) {
        call_when_even(i, output);
    }
}