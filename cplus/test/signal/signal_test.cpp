//
// Created by Gavin on 2022/8/20.
// 信号处理函数的简化过程，以及调用逻辑
// reference: https://blog.csdn.net/pdsleo/article/details/78269905
//

#include "../basic.h"
#include "signal.h"

using namespace std;

typedef void(*handler)(int);

void Fun1(int x) {
    cout << "处理信号1" << endl;
}

void Fun2(int x) {
    cout << "处理信号2" << endl;
}

void show_handler(int sig) {
    cout << "I got signal " << sig << endl;
    int i;
    for (i = 0; i < 5; i++) {
        printf("i = %d\n", i);
        sleep(1);
    }
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

// reference https://weibo01.blog.csdn.net/article/details/81411827?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-81411827-blog-45846389.t0_layer_eslanding_s&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-81411827-blog-45846389.t0_layer_eslanding_s&utm_relevant_index=2
TEST(signale_test, sigaction函数demo) {
    int count = 0;
    pid_t pid;
    struct sigaction newact, oldact;
    newact.sa_handler = SIG_IGN; //这个地方也可以是函数
    sigemptyset(&newact.sa_mask);
    sigaction(SIGINT, &newact, &oldact);

    pid = fork();
    if (pid == 0) {
        while (1) {
            cout << "I am child!" << endl;
            sleep(3);
        }
        return;
    }

    while (1) {
        if (count++ > 3) {
            sigaction(SIGINT, &oldact, NULL);
            cout << "pid = " << pid << endl;
            kill(pid, SIGKILL); //父进程发信号，来杀死子进程
        }

        cout << "I am father!" << endl;
        sleep(3);
    }
    return;
}

TEST(signal_test, sigaction测试信号阻塞等逻辑) {

    int i = 0;
    struct sigaction act, oldact;
    act.sa_handler = show_handler;
    sigaddset(&act.sa_mask, SIGQUIT);
    act.sa_flags = SA_RESETHAND | SA_NODEFER;
    //act.sa_flags = 0;

    sigaction(SIGINT, &act, &oldact);
    while (1) {
        sleep(1);
        cout << "sleeping " << i << endl;
        i++;
    }
}