//
// Created by 宋博文 on 2024/11/4.
//

#include <string>
#include <iostream>
#include <thread>
#include "gtest/gtest.h"

class Person1 {
public:
    std::string name;

    Person1(const std::string &n) : name(n) {
        std::cout << "Person " << name << " created." << std::endl;
    }

    ~Person1() {
        std::cout << "Person " << name << " destroyed." << std::endl;
    }

    void sayHello() const {
        std::cout << "Hello, my name is " << name << std::endl;
    }
};


// 创建一个 std::shared_ptr<Person>，引用计数为 1
std::shared_ptr<Person1> global_v = std::make_shared<Person1>("Alice");

void localCount(const int id) {
    std::shared_ptr<Person1> local_ptr = global_v;
    for (int i = 0; i < 20; ++i) {
        std::cout << "thread " <<  id << " reference count after inner scope: " << local_ptr.use_count() << '\n';
        sleep(1);
    }
}

void localCount2(const int id) {
    for (int i = 0; i < 20; ++i) {
        global_v->sayHello();
        std::cout << "thread " <<  id << " reference count after inner scope: " << global_v.use_count() << '\n';
        sleep(1);
    }
}

void localCount3(const int id) {
    // weakptr
    std::shared_ptr<Person1> local_ptr = global_v;
    for (int i = 0; i < 20; ++i) {
        local_ptr->sayHello();
        std::cout << "thread " <<  id << " reference count after inner scope: " << local_ptr.use_count() << '\n';
        sleep(1);
    }
}

// 正常运行
TEST(smart_pointer_test, 测试主线程reset智能指针对其余线程的影响2) {
    std::thread t1(&localCount3, 1);
    std::thread t2(&localCount3, 2);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "start reset" << std::endl;
    global_v.reset();

    std::cout << "end reset" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(5));
    t1.join();
    t2.join();
}

// interrupted by signal 11:SIGSEGV
TEST(smart_pointer_test, 测试主线程reset智能指针对其余线程的影响) {
    std::thread t1(&localCount2, 1);
    std::thread t2(&localCount2, 2);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "start reset" << std::endl;
    global_v.reset();

    std::cout << "end reset" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(5));
    t1.join();
    t2.join();
}

// 只有在线程中拷贝一个智能指针的副本，才能增加引用
TEST(smart_pointer_test, 测试多线程访问同一个全局变量不会增加引用) {

    global_v->sayHello();

    std::vector<std::thread> threads;

    // 启动多个读取线程
    threads.reserve(10);
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(localCount, i);
    }

    // 等待所有线程完成
    for (auto &t: threads) {
        t.join();
    }

    std::cout << "main thread Reference count after inner scope: " << global_v.use_count() << '\n';


}

TEST(smart_pointer_test, case1) {
    // 创建一个 std::shared_ptr<Person>，引用计数为 1
    std::shared_ptr<Person1> p1 = std::make_shared<Person1>("Alice");
    p1->sayHello();

    {
        // 另一个 std::shared_ptr<Person> 指向同一个对象，引用计数变为 2
        std::shared_ptr<Person1> p2 = p1;
        std::cout << "Reference count inside inner scope: " << p1.use_count() << '\n';

        // 使用 p2 指针访问对象
        p2->sayHello();
    }  // 离开作用域，p2 销毁，引用计数变回 1

    std::cout << "Reference count after inner scope: " << p1.use_count() << '\n';

    p1.reset(); // 不再使用智能指针 或者p1 = nullptr，释放当前对象的所有权
    sleep(1);
    // 离开 main() 函数，p1 销毁，引用计数变为 0，Person 对象被自动释放
    return;
}


