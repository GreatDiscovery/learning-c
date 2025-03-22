//
// Created by 贾芸 on 2025/3/22.
//
#include <gtest/gtest.h>
#include "../basic.h"
#include <thread>

std::atomic<int> data;
std::atomic<bool> ready(false);

void producer() {
    // 生产数据
    data.store(42, std::memory_order_relaxed);

    // 发布数据，确保之前的操作不会被重排序到 release 之后
    ready.store(true, std::memory_order_release);
}

void consumer() {
    // 等待数据发布
    while (!ready.load(std::memory_order_acquire)) {
        // 忙等待
    }

    // 获取数据，确保之后的操作不会被重排序到 acquire 之前
    int value = data.load(std::memory_order_relaxed);
    std::cout << "Data: " << value << std::endl;
    assert(value == 42);
}


TEST(mem_order_test, 测试内存顺序) {
    data.store(10, std::memory_order_relaxed);
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();
}
