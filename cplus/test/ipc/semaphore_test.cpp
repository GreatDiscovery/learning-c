//
// Created by 宋博文 on 2024/7/1.
//

#include "../basic.h"
#include <pthread.h>
#include <semaphore.h>

sem_t sem;
time_t current_time;

void *thread_func_sem(void *arg) {
    sem_wait(&sem);
    time(&current_time);
    printf("Thread %ld entered critical section，timestamp=%ld.\n", (long) arg, current_time);
    // 快速刷新到标准输出
    fflush(stdout);
    sleep(5); // 模拟一些工作
    time(&current_time);
    printf("Thread %ld leaving critical section, timestamp=%ld.\n", (long) arg, current_time);
    fflush(stdout);
    sem_post(&sem);
    return NULL;
}

TEST(semaphore_test, 测试信号量) {
    pthread_t threads[3];
    sem_init(&sem, 0, 1); // 初始化信号量，值为1

    for (long i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_func_sem, (void *) i);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&sem);
}