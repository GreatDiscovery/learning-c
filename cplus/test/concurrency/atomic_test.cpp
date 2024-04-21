//
// Created by 贾芸 on 2023/9/14.
//

#include <thread>
#include <atomic>
#include "../basic.h"

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