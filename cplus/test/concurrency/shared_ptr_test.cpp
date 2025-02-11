//
// Created by 贾芸 on 2025/2/11.
//

#include "../basic.h"
#include "iostream"
#include "gtest/gtest.h"
#include <memory>

class Person3 : public std::enable_shared_from_this<Person3> {
public:
    std::shared_ptr<Person3> get_shared() {
        return shared_from_this(); // ✅ 安全地返回 shared_ptr
    }

    ~Person3() {
        std::cout << "Person 被销毁" << std::endl;
    }
};

TEST(shared_ptr_test, 测试enable_shared_from_this) {
    std::shared_ptr<Person3> p1 = std::make_shared<Person3>();
    std::shared_ptr<Person3> p2 = p1->get_shared(); // 共享相同的控制块

    std::cout << "p1.use_count() = " << p1.use_count() << std::endl; // 输出 2
    std::cout << "p2.use_count() = " << p2.use_count() << std::endl; // 输出 2

    ASSERT_EQ(p1.use_count(), p2.use_count());
}
