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


TEST(rand_test3, 测试rand函数在一秒之内生成的数据是否相同) {
    std::unordered_set<int> numbers;
    srand(time(NULL));
    for (int i = 0; i < 1000; ++i) {
        int rand_id = rand();
//        printf("%d\n", rand_id);
        if (!numbers.insert(rand_id).second) {
            std::cout << "Thread " << std::this_thread::get_id() << " found a duplicate: " << rand_id << std::endl;
        }
    }
    std::cout << "end" << std::endl;
}

std::unordered_set<int> global_set;
std::mutex myMutex;
int total = 0;
int duplicate = 0;

void threadFunc3() {
    srand(time(NULL));
    for (int i = 0; i < 100; ++i) {
        int rand_id = rand();
        myMutex.lock();
        total++;
        if (!global_set.insert(rand_id).second) {
            duplicate++;
            std::cout << "Thread " << std::this_thread::get_id() << " found a duplicate: " << rand_id << std::endl;
        }
        myMutex.unlock();
    }
}


//结论，srand基于ms生成时间戳，所以同一秒内，大概率生成相同的数字
TEST(rand_test4, 测试多线程函数在一秒之内生成的数据是否相同) {
    std::vector<std::thread> threads;
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threads.emplace_back(threadFunc3);
    }

    for (auto &thread: threads) {
        thread.join();
    }
    std::cout << "total=" << total << ", duplicate=" << duplicate << std::endl;
}