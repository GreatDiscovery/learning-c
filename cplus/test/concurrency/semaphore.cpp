//
// Created by 宋博文 on 2024/8/3.
//

#include <semaphore.h>
#include "../basic.h"

#define NUM_THREADS 4
#define MAX_CONCURRENT 3

// 创建一个信号量，允许最多 MAX_CONCURRENT 个线程同时访问
sem_t semaphore;

// 共享资源
int shared_resource = 0;

void *worker(void *arg) {
    int thread_id = *((int *) arg);

    // 请求信号量
    sem_wait(&semaphore);

    printf("Thread %d is working.\n", thread_id);
    // 模拟工作
    sleep(1);

    // 修改共享资源
    shared_resource++;

    printf("Thread %d is done. Shared resource: %d\n", thread_id, shared_resource);

    // 释放信号量
    sem_post(&semaphore);

    return NULL;
}


TEST(semaphore_test, semaphore测试) {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // 初始化信号量，初始值为 MAX_CONCURRENT
    if (sem_init(&semaphore, 0, MAX_CONCURRENT) != 0) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    // 创建线程
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        if (pthread_create(&threads[i], NULL, worker, &thread_ids[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // 等待所有线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 销毁信号量
    sem_destroy(&semaphore);

}