//
// Created by 贾芸 on 2023/9/14.
//

#include <thread>
#include <atomic>
#include <semaphore> // C++20 信号量
#include "../basic.h"

// atomic operation
#define ngx_atomic_fetch_add(value, add)                                      \
    __sync_fetch_and_add(value, add)

#define ngx_atomic_cmp_set(lock, old, set)                                    \
    __sync_bool_compare_and_swap(lock, old, set)

std::atomic<int> atomicValue(0);

void incrementValue() {
    for (int i = 0; i < 1000; ++i) {
        ++atomicValue;
    }
}

TEST(atomic_test, 原子操作) {
    std::thread t1(incrementValue);
    std::thread t2(incrementValue);
    t1.join();
    t2.join();
    std::cout << "Final value: " << atomicValue << std::endl;
}

int a = 0;
void* incrementValue2(void *arg) {
    UNUSED(arg);
    for (int i = 0; i < 1000; i++) {
        ngx_atomic_fetch_add(&a, 1);
    }
    return NULL;
}


TEST(atomic_test, C语言的原子操作) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, incrementValue2, NULL);
    pthread_create(&t2, NULL, incrementValue2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("a=%d\n", a);
    assert(a == 2000);
}

std::atomic<int> counter(0);

void increment() {
    for (int i = 0; i < 1000000; ++i) {
        // 仅保证原子性，不提供线程间的同步或顺序保证,能保证修改顺序一致性
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

TEST(atomic_test, 测试memory_order) {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Counter: " << counter.load(std::memory_order_relaxed) << std::endl;
}


std::atomic<bool> flag1(false);
std::atomic<bool> flag2(false);
std::atomic<bool> flag3(false);
std::atomic<bool> flag4(false);

std::binary_semaphore startSignal(0);

void thread1() {
    startSignal.acquire(); // 等待信号
    flag1.store(true, std::memory_order_seq_cst); // 设置 flag1
    if (flag2.load(std::memory_order_seq_cst)) { // 检查 flag2
        std::cout << "Thread 1: flag2 is true" << std::endl;
    }
}

void thread2() {
    startSignal.acquire(); // 等待信号
    flag2.store(true, std::memory_order_seq_cst); // 设置 flag2
    if (flag1.load(std::memory_order_seq_cst)) { // 检查 flag1
        std::cout << "Thread 2: flag1 is true" << std::endl;
    }
}

void thread3() {
    startSignal.acquire(); // 等待信号
    flag3.store(true, std::memory_order_relaxed);
    if (flag4.load(std::memory_order_relaxed)) {
        std::cout << "Thread 3: flag4 is true" << std::endl;
    }
}

void thread4() {
    startSignal.acquire(); // 等待信号
    flag4.store(true, std::memory_order_relaxed);
    if (flag3.load(std::memory_order_relaxed)) {
        std::cout << "Thread 4: flag3 is true" << std::endl;
    }
}

TEST(atomic_test, 测试memory_order_seq_cst) {
    std::thread t1(thread1);
    std::thread t2(thread2);
    // 模拟准备工作
    std::this_thread::sleep_for(std::chrono::seconds(1));
    startSignal.release(2); // 释放两个信号，允许两个线程同时运行

    t1.join();
    t2.join();

    // 模拟准备工作
    std::this_thread::sleep_for(std::chrono::seconds(1));
    startSignal.release(2); // 释放两个信号，允许两个线程同时运行
    std::thread t3(thread3);
    std::thread t4(thread4);
    t3.join();
    t4.join();
}