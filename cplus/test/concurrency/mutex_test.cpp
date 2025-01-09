//
// Created by Gavin on 2023/3/20.
// unix系统编程，互斥量测试
//

#include "../basic.h"
#include <thread>
#include <shared_mutex>


std::shared_mutex mutex;
int shared_data = 0;

void reader(const int id) {
    std::shared_lock lock(mutex); // 共享锁
    std::cout << "Reader " << id << " reads shared_data: " << shared_data << "\n";
}

void writer(const int id) {
    std::unique_lock lock(mutex); // 独占锁
    ++shared_data;
    std::cout << "Writer " << id << " modifies shared_data to: " << shared_data << "\n";
}

TEST(share_lock, 测试读写锁) {
    std::vector<std::thread> threads;

    // 启动多个读取线程
    threads.reserve(10);
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(reader, i);
    }

    // 启动一个写入线程
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(writer, i);
    }

    // 等待所有线程完成
    for (auto &t: threads) {
        t.join();
    }
}

std::mutex checking_mutex_;
int total = 0;

void addNum() {
    for (int i = 0; i < 10; i++) {
        {
            std::lock_guard<std::mutex> lock(checking_mutex_);
            total += 1;
        }
    }
}


TEST(mutext_test, 测试lock) {
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(addNum);
    }

    for (auto &thread: threads) {
        thread.join();
    }
    std::cout << total << std::endl;
}

struct foo {
    int f_count;
    pthread_mutex_t f_lock;
    int f_id;
};

struct foo *foo_alloc(int id) {
    struct foo *fp;
    if ((fp = static_cast<foo *>(malloc(sizeof(struct foo)))) != NULL) {
        fp->f_count = 1;
        fp->f_id = id;
        if (pthread_mutex_init(&fp->f_lock, nullptr) != 0) {
            free(fp);
            return nullptr;
        }
    }
    return fp;
}
