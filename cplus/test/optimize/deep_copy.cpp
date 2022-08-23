//
// Created by Gavin on 2022/8/21.
//

#include <list>
#include "../basic.h"
#include "chrono"

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
// 如何使用Chrono, https://zhuanlan.zhihu.com/p/269982520
    using namespace std::chrono;
    TEST(move_test, move语义和作用) {
        {
            std::list<std::string> tokens;
            for (int i = 0; i < 10000000; i++) {
                tokens.push_back(std::to_string(i));
            }
            time_point<system_clock> start = system_clock::now();
            std::list<std::string> t = tokens;
            time_point<system_clock> end = system_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            std::cout << "copy construct elapsed time: " << elapsed.count() << "s" << std::endl;
        }
        std::list<std::string> tokens;
        for (int i = 0; i < 10000000; i++) {
            tokens.push_back(std::to_string(i));
        }
        time_point<system_clock> start = system_clock::now();
        std::list<std::string> t = std::move(tokens);
        time_point<system_clock> end = system_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "move elapsed time: " << elapsed.count() << "s" << std::endl;
    }
}