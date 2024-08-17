//
// Created by Gavin on 2023/3/20.
// unix系统编程，互斥量测试
//

#include "../basic.h"
#include <thread>

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

