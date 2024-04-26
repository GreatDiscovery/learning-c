//
// Created by 贾芸 on 2024/4/26.
//

#include <gtest/gtest.h>
#include <iostream>

using namespace std;

/**
 * 智能指针可以管理对象的声明周期，在对象声明周期结束时，可以调用delete释放内存，防止内存泄漏
 * 左值（lvalue）：左值是指可以放在赋值运算符左边的表达式，即具有内存地址的表达式。左值可以是变量、数组元素、结构体成员等。左值可以取地址，并且可以被修改。
右值（rvalue）：右值是指不能放在赋值运算符左边的表达式，即不具有内存地址的临时值。右值通常是字面常量、临时变量、函数返回值等。右值不能被取地址，因为它们没有明确的内存位置，并且通常不能被修改。
 */
TEST(unique_ptr_case, 使用智能指针) {
    unique_ptr<string> p1(new string("I'm Li Ming!"));
    unique_ptr<string> p2(new string("I'm age 22."));

    cout << "p1：" << p1.get() << endl;
    cout << "p2：" << p2.get() << endl;
    cout << "p1: " << *p1 << endl;
    cout << "p2: " << *p2 << endl;

//    p1 = p2;                    // 禁止左值赋值
//    unique_ptr<string> p3(p2);    // 禁止左值赋值构造
//
//    unique_ptr<string> p3(std::move(p1));
    p1 = std::move(p2);    // 使用move把左值转成右值就可以赋值了，效果和auto_ptr赋值一样

    cout << "p1 = p2 赋值后：" << endl;
    cout << "p1：" << p1.get() << endl;
    cout << "p2：" << p2.get() << endl;
}

class Person {
public:
    Person(int v) {
        this->no = v;
        cout << "构造函数 \t no = " << this->no << endl;
    }

    ~Person() {
        cout << "析构函数 \t no = " << this->no << endl;
    }

private:
    int no;
};

// 仿函数，内存删除
class DestructPerson {
public:
    void operator()(Person *pt) {
        cout << "DestructPerson..." << endl;
        delete pt;
    }
};


TEST(shared_ptr_case, 使用智能指针_Test) {
    shared_ptr<Person> sp1;

    shared_ptr<Person> sp2(new Person(2));

// 获取智能指针管控的共享指针的数量	use_count()：引用计数
    cout << "sp1	use_count() = " << sp1.use_count() << endl;
    cout << "sp2	use_count() = " << sp2.use_count() << endl << endl;

// 共享
    sp1 = sp2;

    cout << "sp1	use_count() = " << sp1.use_count() << endl;
    cout << "sp2	use_count() = " << sp2.use_count() << endl << endl;

    shared_ptr<Person> sp3(sp1);
    cout << "sp1	use_count() = " << sp1.use_count() << endl;
    cout << "sp2	use_count() = " << sp2.use_count() << endl;
    cout << "sp2	use_count() = " << sp3.use_count() << endl << endl;

}