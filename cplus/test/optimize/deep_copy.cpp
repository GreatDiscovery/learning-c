//
// Created by Gavin on 2022/8/21.
//

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
}