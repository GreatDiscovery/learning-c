//
// Created by Gavin on 2022/9/9.
//

#include "../basic.h"

using namespace std;

template<typename R, typename T, typename U>
R add(T t, U u) {
    return t + u;
}

// T 和 U 可能没有无参构造函数
template<typename T, typename U>
decltype(T() + U()) add2(T t, U u) {
    return t + u;
}

// 该方法是前置语法
template<typename T, typename U>
decltype((*(T*)0) + (*(U*)0)) add3(T t, U u) {
    return t + u;
}

template <typename T, typename U>
auto add4(T t, U u) -> decltype(t + u) {
    return t + u;
}

TEST(decltype_test, 返回类型后置语法) {
    int a = 1;
    float b = 2.0;
    auto c = add<decltype(a + b)>(a, b);
    cout << typeid(c).name() << endl;

    auto d = add4(a, b);
    cout << typeid(d).name() << endl;
}