//
// Created by Gavin on 2022/8/18.
// 测试拷贝构造函数,如何区别拷贝构造和赋值，https://www.cnblogs.com/wangguchangqing/p/6141743.html。
//拷贝构造函数使用传入对象的值生成一个新的对象的实例，而赋值运算符是将对象的值复制给一个已经存在的实例
//

#include "../basic.h"

using namespace std;

class Person {
public:
    Person() {}

    Person(const Person &p) : age(p.age), name(p.name) {
        cout << "copy constructor!" << endl;
    }

    Person &operator=(const Person &p) {
        cout << "assign" << endl;
        return *this;
    }

private:
    int age;
    string name;
};

void f1(Person p) {
    return;
}

Person f2() {
    Person p;
    return p;
}

// 需要设置-fno-elide-constructors关闭编译器优化
TEST(copy_constructor_test, 拷贝构造和赋值比较) {
    cout << "1. ---------------" << endl;
    Person p;
    Person p1 = p;    // 1
    cout << "2. ---------------" << endl;
    Person p2;
    p2 = p;           // 2
    cout << "3. ---------------" << endl;
    f1(p2);            // 3
    cout << "4. ---------------" << endl;
    p2 = f2();        // 4
    cout << "5. ---------------" << endl;
    /**
    *如果没有优化（即严格按照标准语义）：
        f2() 返回 p，会触发拷贝构造函数来将局部变量 p 拷贝到返回值。
        Person p3 = f2(); 触发拷贝构造函数，将 f2() 的返回值初始化 p3
    编译器优化后，局部变量 p 的内存位置会被直接用作 p3
     */
    Person p3 = f2(); // 5
    cout << "end---------------" << endl;
}