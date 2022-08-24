//
// Created by Gavin on 2022/8/24.
// 程序自动区分左值和右值,forward主要解决传入了右值，但是被转换成了左值的情况
// 另外需要区分move和forward。move会无脑转为右值，forward会根据入参判断。
// reference: https://blog.csdn.net/u012508160/article/details/79218936?spm=1001.2101.3001.6650.10&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-10-79218936-blog-120148193.t5_layer_eslanding_A_4&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-10-79218936-blog-120148193.t5_layer_eslanding_A_4&utm_relevant_index=16
//

#include "../basic.h"

template<typename T>
void PrintT(T& t) {
    std::cout << "left value" << std::endl;
}

template<typename T>
void PrintT(T&& t) {
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