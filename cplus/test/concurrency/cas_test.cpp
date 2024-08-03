//
// Created by 宋博文 on 2024/8/2.
//

#include "../basic.h"
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 10
#define TOTAL 100000000

#define ngx_atomic_cmp_set(lock, old, set)                                    \
    __sync_bool_compare_and_swap(lock, old, set)

#define ngx_atomic_fetch_add(value, add)                                      \
    __sync_fetch_and_add(value, add)

// work for x86
// #define ngx_cpu_pause()             __asm__ ("pause")


static inline void ngx_cpu_pause(void) {
    // arm architecture
    __asm__ __volatile__("wfe");
}

/**
 * 设计10个线程并发相加到100000的耗时
 * 比较实用传统方式mutex和atomic之间谁耗时更低
 * cas加锁方式从nginx学习来的
 */

int shared_variable1 = 0;
int shared_variable2 = 0;
volatile unsigned long pid_lock;
volatile unsigned long waitNum;
sem_t sem1;

pthread_mutex_t mutex2;

void thread_mutex_cost();

void thread_cas_cost();

void *thread_function1(void *arg);

void *thread_function2(void *arg);

void thread_lock();

void thread_unlock();

TEST(cas_test, 比较加锁和无锁之间性能差异) {
    thread_mutex_cost();
    thread_cas_cost();
}

void thread_mutex_cost() {
    time_t start, end;
    double elapsed;

    pthread_mutex_init(&mutex2, NULL);
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, thread_function1, NULL) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    start = time(NULL);
    // 等待线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join failed");
            exit(EXIT_FAILURE);
        }
    }

    printf("Final value of shared_variable: %d\n", shared_variable1);
    end = time(NULL);
    elapsed = difftime(end, start);

    printf("thread_mutex_cost Elapsed time: %f seconds\n", elapsed);
    // 销毁互斥锁
    pthread_mutex_destroy(&mutex2);
}

void *thread_function1(void *arg) {
    while (true) {
        pthread_mutex_lock(&mutex2);
        if (shared_variable1 >= TOTAL) {
            pthread_mutex_unlock(&mutex2);
            break;
        }
        shared_variable1++;
        pthread_mutex_unlock(&mutex2);
    }
}

void *thread_function2(void *arg) {
    while (true) {
        thread_lock();
        if (shared_variable2 >= TOTAL) {
            thread_unlock();
            break;
        }
        shared_variable2++;
        thread_unlock();
    }
}

void thread_cas_cost() {
    time_t start, end;
    double elapsed;

    pid_lock = 0;
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, thread_function2, NULL) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    if (sem_init(&sem1, 1, 0) == -1) {
        perror("sem_init() failed");
        exit(EXIT_FAILURE);
    }
    start = time(NULL);
    // 等待线程完成
    // 等待线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join failed");
            exit(EXIT_FAILURE);
        }
    }

    printf("Final value of shared_variable: %d\n", shared_variable2);
    end = time(NULL);
    elapsed = difftime(end, start);

    printf("thread_cas_cost Elapsed time: %f seconds\n", elapsed);
    free((void *) pid_lock);
}

void thread_lock() {
    int n;
    for (;;) {
        // 1. 自旋获取锁
        if (pid_lock == 0 && ngx_atomic_cmp_set(&pid_lock, 0, pthread_self())) {
            return;
        }
        // 2. 让出cpu时间等待获取锁
        for (n = 1; n < 2048; n <<= 1) {
            for (int i = 0; i < n; i++) {
                ngx_cpu_pause();
            }
            if (pid_lock == 0 && ngx_atomic_cmp_set(&pid_lock, 0, pthread_self())) {
                return;
            }
        }
        // 3. semaphore获取锁
        ngx_atomic_fetch_add(&waitNum, 1);
        if (pid_lock == 0 && ngx_atomic_cmp_set(&pid_lock, 0, pthread_self())) {
            return;
        }
        while (sem_wait(&sem1) == -1) {
            perror("sem_wait() failed while waiting on shmtx");
            break;
        }
        // 4. 实在获取不到锁，等待
        sched_yield();
    }
}

void thread_unlock() {
    if (ngx_atomic_cmp_set(&pid_lock, pthread_self(), 0)) {
        // release semaphore
        for (;;) {
            if ((long) waitNum <= 0) {
                return;
            }

            if (ngx_atomic_cmp_set(&waitNum, waitNum, waitNum - 1)) {
                break;
            }
        }

        if (sem_post(&sem1) == -1) {
            perror("sem_post() failed while wake shmtx");
        }

    }
}