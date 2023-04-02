//
// Created by Gavin on 2023/3/20.
//

#include "../basic.h"

using namespace std;

void print_ids(const char *s) {
    pid_t pid;
    pthread_t tid;
    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long) pid, (unsigned long) tid, (unsigned long) tid);
}

void *thr_fn(void *arg) {
    print_ids("new thread: ");
    return ((void *) 0);
}

TEST(pthread_test, 多线程测试) {
    int err;
    pthread_t ntid;
    err = pthread_create(&ntid, NULL, thr_fn, NULL);
    if (err != 0) {
        exit(1);
    }
    print_ids("main thread: ");
    sleep(3);
    exit(0);
}
