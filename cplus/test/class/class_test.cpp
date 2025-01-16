//
// Created by 宋博文 on 2024/4/21.
//

#include <iostream>
#include <gtest/gtest.h>

class MyClass {
public:
    explicit MyClass(int x) : value(x) {}

    // const 方法：不能修改成员变量
    int getValue() const;

private:
    int value;
};

void printValue(const MyClass &obj) {
    std::cout << "Value: " << obj.getValue() << std::endl;
}

int MyClass::getValue() const {
    return value;
}

TEST(MyClass, explicit_test) {
    // explicit 是一个关键字，用于声明单参数构造函数为显式构造函数，这意味着该构造函数不会自动进行隐式类型转换。
//    MyClass obj1 = 10; // 错误，不能进行隐式类型转换
//    printValue(obj1);
    MyClass obj2(20); // 正确，直接调用构造函数进行初始化
    printValue(obj2);
}