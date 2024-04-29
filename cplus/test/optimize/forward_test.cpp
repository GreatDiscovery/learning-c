//
// Created by Gavin on 2022/8/24.
// 程序自动区分左值和右值,forward主要解决传入了右值，但是被转换成了左值的情况
// 另外需要区分move和forward。move会无脑转为右值，forward会根据入参判断。
// reference: https://blog.csdn.net/u012508160/article/details/79218936?spm=1001.2101.3001.6650.10&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-10-79218936-blog-120148193.t5_layer_eslanding_A_4&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-10-79218936-blog-120148193.t5_layer_eslanding_A_4&utm_relevant_index=16
//

#include "../basic.h"

template<typename T>
void PrintT(T &t) { // 当同时有Print(T &t)和PrintT(T &&t)时，由于传入形参Print(x)，就会命中左值引用的PrintT(T &t)
    std::cout << "left value" << std::endl;
}

template<typename T>
void PrintT(T &&t) {
    std::cout << "right value" << std::endl;
}

template<typename T>
void TestForward(T &&v) {
    PrintT(v);
    PrintT(std::forward<T>(v));
    PrintT(std::move(v));
}


TEST(forward_test, 转发优化) {
    TestForward(1);
    std::cout << "==============================" << std::endl;
    int x = 1;
    TestForward(x);
    std::cout << "==============================" << std::endl;
    TestForward(std::forward<int>(x));
    std::cout << "==============================" << std::endl;
}

using namespace std;

template<typename T>
void func(T &&param) {
    if (std::is_same<string, T>::value)
        std::cout << "string" << std::endl;
    else if (std::is_same<string &, T>::value)
        std::cout << "string&" << std::endl;
    else if (std::is_same<string &&, T>::value)
        std::cout << "string&&" << std::endl;
    else if (std::is_same<int, T>::value)
        std::cout << "int" << std::endl;
    else if (std::is_same<int &, T>::value)
        std::cout << "int&" << std::endl;
    else if (std::is_same<int &&, T>::value)
        std::cout << "int&&" << std::endl;
    else
        std::cout << "unkown" << std::endl;
}

int getInt() {
    return 10;
}

// 详解move和forward：https://blog.csdn.net/newchenxf/article/details/117995131
//T& & -> T& （对左值引用的左值引用是左值引用）
//T& && -> T& （对左值引用的右值引用是左值引用）
//T&& & ->T& （对右值引用的左值引用是左值引用）
//T&& && ->T&& （对右值引用的右值引用是右值引用）
TEST(forward_test, 引用折叠) {
    int x = 1;
    func(1); // 传递参数是右值 T推导成了int, 所以是int&& param, 右值引用
    func(x); // 传递参数是左值 T推导成了int&, 所以是int& && param, 折叠成 int&,左值引用
    func(getInt());// 参数getInt是右值 T推导成了int, 所以是int&& param, 右值引用
}

template<typename T>
void foo(T &&param) {
    if (std::is_rvalue_reference<decltype(param)>::value)
        std::cout << "rvalue reference\n";
    else std::cout << "lvalue reference\n";
}

// case: https://zhuanlan.zhihu.com/p/469607144
TEST(forward_test, 引用折叠2) {
    int a = 0;
    foo(a);
    foo(std::move(a));
}

class A {
public:
    A() {}

    A(size_t size) : size(size), array((int *) malloc(size)) {
        std::cout
                << "create Array，memory at: "
                << array << std::endl;

    }

    ~A() {
        free(array);
    }

    A(A &&a) : array(a.array), size(a.size) {
        a.array = nullptr;
        std::cout
                << "Array moved, memory at: "
                << array
                << std::endl;
    }

    A(A &a) : size(a.size) {
        array = (int *) malloc(a.size);
        for (int i = 0; i < a.size; i++)
            array[i] = a.array[i];
        std::cout
                << "Array copied, memory at: "
                << array << std::endl;
    }

    size_t size;
    int *array;
};

template<typename T>
void warp(T &&param) {
    if (std::is_rvalue_reference<decltype(param)>::value) {
        std::cout << "param is rvalue reference\n";
    } else std::cout << "param is lvalue reference\n";
    A y = A(param); // 右值如果不使用forward转发，由于形参存在实际的地址空间则进一步的函数调用会被当作左值
    A z = A(std::forward<T>(param)); // 防止右值变为左值，
}

TEST(forward_test, 完美转发Test) {
    A a = A(100);
    warp(std::move(a)); // 右值在函数转发过程中，容易重新变为左值。
}