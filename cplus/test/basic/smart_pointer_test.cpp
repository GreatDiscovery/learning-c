//
// Created by 宋博文 on 2024/11/4.
//

#include <string>
#include <iostream>
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

