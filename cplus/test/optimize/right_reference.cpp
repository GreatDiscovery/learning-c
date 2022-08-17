//
// Created by Gavin on 2022/8/15.
//右值引用, &&变量名称，但是demo演示的很令人疑惑
//

#include "../basic.h"

using namespace std;

int g_constructCount = 0;
int g_copyConstructCount = 0;
int g_destructCount = 0;

struct A {
    A() {
        cout << "construct: " << ++g_constructCount << endl;
    }

    A(const A &a) {
        cout << "copy construct: " << ++g_copyConstructCount << endl;
    }

    ~A() {
        cout << "destruct: " << ++g_destructCount << endl;
    }
};

A GetA();

A GetA() {
    return A();
}


TEST(right_test, 右值引用特性) {
    // 这里优化器会把临时对象优化掉，只调用了构造函数，而没有调用拷贝构造函数
    A a = GetA();

    A newNewA = a;

    A&& newA = GetA();



}
