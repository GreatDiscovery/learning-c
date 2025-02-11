//
// Created by 宋博文 on 2024/11/4.
//

#include <string>
#include <iostream>
#include <thread>
#include <atomic>
#include <shared_mutex>
#include "gtest/gtest.h"

std::mutex cout_mutex;

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
        sleep(10);
        // 第二次查询name为空，因为主线程已经调用析构函数了
        std::cout << "Hello, my name is " << name << std::endl;
    }
};

// Trivially Copyable 类型，不涉及复杂的构造、析构或虚函数
class Person2 {
public:
    int age;

    Person2(int a) : age(a) {
    }

    ~Person2() = default;
};


// 创建一个 std::shared_ptr<Person>，引用计数为 1
std::shared_ptr<Person1> global_v = std::make_shared<Person1>("Alice");
// std::atomic<std::shared_ptr<Person1>> global_automic_data;

std::shared_ptr<Person1> getGlobalV() {
    return global_v;
}


void localCount(const int id) {
    std::shared_ptr<Person1> local_ptr = global_v;
    for (int i = 0; i < 20; ++i) {
        std::cout << "thread " << id << " reference count after inner scope: " << local_ptr.use_count() << '\n';
        sleep(1);
    }
}

void localCount2(const int id) {
    for (int i = 0; i < 20; ++i) {
        sleep(1);
        global_v->sayHello();
    }
}

void localCount3(const int id) {
    // weakptr
    std::shared_ptr<Person1> local_ptr = global_v;
    for (int i = 0; i < 20; ++i) {
        local_ptr->sayHello();
        std::cout << "thread " << id << " reference count after inner scope: " << local_ptr.use_count() << '\n';
        sleep(1);
    }
}

// void localCount4(const int id) {
//     for (int i = 0; i < 20; ++i) {
//         global_automic_data.load()->sayHello();
//         std::cout << "thread " <<  id << " reference count after inner scope: " << global_automic_data.load().use_count() << '\n';
//         sleep(1);
//     }
// }

void localCount5(const int id) {
    for (int i = 0; i < 20; ++i) {
        getGlobalV()->sayHello();
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



// 只有在线程中拷贝一个智能指针的副本，才能增加引用。这种不增加引用个数，只判断对象是否活着的引用是weak_ptr
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
    p1->sayHello(); {
        // 另一个 std::shared_ptr<Person> 指向同一个对象，引用计数变为 2
        std::shared_ptr<Person1> p2 = p1;
        std::cout << "Reference count inside inner scope: " << p1.use_count() << '\n';

        // 使用 p2 指针访问对象
        p2->sayHello();
    } // 离开作用域，p2 销毁，引用计数变回 1

    std::cout << "Reference count after inner scope: " << p1.use_count() << '\n';

    p1.reset(); // 不再使用智能指针 或者p1 = nullptr，释放当前对象的所有权
    sleep(1);
    // 离开 main() 函数，p1 销毁，引用计数变为 0，Person 对象被自动释放
    return;
}


// Trivially Copyable 类型，不涉及复杂的构造、析构或虚函数。智能指针涉及复杂的析构函数，编译时会报错
// TEST(smart_pointer_test, 多线程安全析构使用方式) {
//
//     global_automic_data.store(std::make_shared<Person1>("jiayun"));
//
//     global_automic_data.load()->sayHello();
//
//     std::vector<std::thread> threads;
//
//     // 启动多个读取线程
//     threads.reserve(10);
//     for (int i = 0; i < 5; ++i) {
//         threads.emplace_back(localCount4, i);
//     }
//
//     // 等待所有线程完成
//     for (auto &t: threads) {
//         t.join();
//     }
//
//     std::cout << "main thread Reference count after inner scope: " << global_automic_data.load().use_count() << '\n';
// }

class MyClass {
public:
    int data;

    MyClass(int val) : data(val) {
    }
};

TEST(smart_pointer_test, atomic测试) {
    std::atomic<MyClass> atomic_obj(MyClass(42)); // OK

    // Accessing atomic value (dangerous without proper synchronization, just for illustration)
    std::cout << atomic_obj.load().data << std::endl;

    std::atomic<Person2> a(Person2(1));
    std::cout << a.load().age << std::endl;
    // 编译报错
    // std::atomic<std::shared_ptr<Person2>> b(std::make_shared<Person2>(2));
    // std::cout << b.load().get()->age << std::endl;
}

// 能够正常运行，函数会拷贝一份智能指针的副本
TEST(smart_pointer_test, 测试函数是否也会拷贝一份智能指针的副本) {
    getGlobalV()->sayHello();

    std::vector<std::thread> threads;

    // 启动多个读取线程
    threads.reserve(10);
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(localCount5, i);
    }

    std::cout << "start reset" << std::endl;
    std::shared_ptr<Person1> old_ptr = global_v;
    global_v = std::make_shared<Person1>("Bob");
    old_ptr.reset();

    std::cout << "end reset" << std::endl;

    // 等待所有线程完成
    for (auto &t: threads) {
        t.join();
    }

    std::cout << "main thread Reference count after inner scope: " << global_v.use_count() << '\n';
}


// interrupted by signal 11:SIGSEGV
TEST(smart_pointer_test, 测试主线程reset智能指针对其余线程的影响) {
    std::vector<std::thread> threads;

    // 启动多个读取线程
    threads.reserve(20);
    for (int i = 0; i < 20; ++i) {
        threads.emplace_back(localCount2, i);
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "start reset" << std::endl;
    std::shared_ptr<Person1> old_ptr = global_v;
    global_v = std::make_shared<Person1>("Bob");
    old_ptr.reset();

    std::cout << "end reset" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(5));

    // 等待所有线程完成
    for (auto &t: threads) {
        t.join();
    }
}