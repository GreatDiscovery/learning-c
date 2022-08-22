//
// Created by Gavin on 2022/8/21.
//

#include <list>
#include "../basic.h"

namespace deep_copy {

    class A {
    public:
        A() : m_ptr(new int(0)) {
            std::cout << "constructor" << std::endl;
        }

        A(const A& a) : m_ptr(a.m_ptr) {
            std::cout << "copy constructor" << std::endl;
        }

        ~A() {
            std::cout << "destructor" << std::endl;
            delete m_ptr;
        }

        int *getM_ptr() {
            return m_ptr;
        }

    private:
        int *m_ptr;
    };

    A Get(bool flag) {
        A a;
        A b = a;
        std::cout << "a.m_ptr:" << a.getM_ptr() << std::endl;
        std::cout << "b.m_ptr:" << b.getM_ptr() << std::endl;
        if (flag) {
            return a;
        } else {
            return b;
        }
    }

    TEST(deep_copy_test, 重复删除指针报错) {
        A a = Get(false);
    }

//    应用move 语义调用构造函数，就避免了拷贝，提高了程序性能。当一个对象内部有较大的堆内存或者动态数组时，
//    很有必要写move 语义的拷贝构造函数和赋值函数，避免无谓的深拷贝，以提高性能
    TEST(move_test, move语义和作用) {
        {
            std::list<std::string> tokens;
            // todo 计时比较
            std::list<std::string> t = tokens;
        }
        std::list<std::string> tokens;
        std::list<std::string> t = std::move(tokens);
    }
}