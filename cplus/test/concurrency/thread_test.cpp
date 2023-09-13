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

pthread_cond_t cond;
pthread_mutex_t mutex1;
int flag = 0;

void *thread1(void *arg);

void *thread2(void *arg);

TEST(pthread_test, 多线程条件变量) {
    pthread_t t1, t2;
    pthread_mutex_init(&mutex1, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex1);
    pthread_cond_destroy(&cond);
}

/**
 * pthread_cond_wait必须与pthread_mutex配套使用。pthread_cond_wait函数一进入wait状态就会自动release mutex，
 * 当其他线程通过pthread_cond_signal或pthread_cond_broadcast把该线程唤醒时，该线程又自动获得该mutex。
 * pthread_cond_wait可能会出现虚假唤醒的情况，即线程在没有收到信号的情况下被唤醒。
 * 因此，在使用pthread_cond_wait时，需要在循环中检查条件变量是否满足，以避免虚假唤醒导致的错误
 * @param arg
 * @return
 */
void *thread1(void *arg) {
    pthread_mutex_lock(&mutex1);
    while (flag == 0) {
        pthread_cond_wait(&cond, &mutex1);
    }
    printf("Thread 1 is awake!\n");
    pthread_mutex_unlock(&mutex1);
}

void *thread2(void *arg) {
    pthread_mutex_lock(&mutex1);
    flag = 1;
    printf("Thread 2 signaled!\n");
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex1);
}