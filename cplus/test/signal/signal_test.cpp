//
// Created by Gavin on 2022/8/20.
// 信号处理函数的简化过程，以及调用逻辑
// reference: https://blog.csdn.net/pdsleo/article/details/78269905
//

#include "../basic.h"
//#include "signal.h"

using namespace std;

typedef void(*handler)(int);

void Fun1(int x) {
    cout << "处理信号1" << endl;
}

void Fun2(int x) {
    cout << "处理信号2" << endl;
}

void (*signal(int signum, void(*handler)(int)))(int) {
    return handler;
}

TEST(signal_test, 信号处理函数的简化) {
    // 注册了不同的信号处理函数指针，每个信号可以对应一个传入的信号处理函数
    void (*pFun1)(int) = signal(1, Fun1);
    void (*pFunc2)(int) = signal(2, Fun2);
    pFun1(1);
    pFunc2(2);
}

// todo signaction函数
TEST(signale_test, signaction函数demo) {

}