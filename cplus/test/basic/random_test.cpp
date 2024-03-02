//
// Created by 贾芸 on 2024/3/2.
//

#include "../basic.h"
#include "string"
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <unordered_set>

// 定义线程数和生成的随机数总数
const int THREAD_COUNT = 5;
const int RAND_COUNT = 10000;

// 模拟每个线程的工作
void threadFunc() {
    std::unordered_set<int> numbers;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, RAND_MAX);

    for (int i = 0; i < RAND_COUNT; ++i) {
        int randNum = dis(gen);
        if (!numbers.insert(randNum).second) {
            std::cout << "Thread " << std::this_thread::get_id() << " found a duplicate: " << randNum << std::endl;
        }
    }
}


// 统计相同值出现的次数
std::unordered_map<int, int> countMap;

void threadFunc2() {
    // 生成随机数并统计
    for (int i = 0; i < 100; ++i) {
        int randomNum = rand();
        ++countMap[randomNum];
    }
}

// fixme
TEST(rand_test, 测试并发调用random函数冲突的概率) {
    std::vector<std::thread> threads;
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threads.emplace_back(threadFunc);
    }

    for (auto &thread: threads) {
        thread.join();
    }
}



TEST(rand_test2, 测试随机数字冲突的概率2) {
    std::vector<std::thread> threads;
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threads.emplace_back(threadFunc2);
    }

    for (auto &thread: threads) {
        thread.join();
    }
    // 输出相同值出现的次数
    for (const auto &pair: countMap) {
        if (pair.second > 1) {
            std::cout << "随机数 " << pair.first << " 出现了 " << pair.second << " 次" << std::endl;
        }
    }
}