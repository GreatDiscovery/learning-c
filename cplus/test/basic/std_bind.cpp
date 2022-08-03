//
// Created by Gavin on 2022/8/4.
//

#include "iostream"
#include "gtest/gtest.h"
#include "functional"
// bind把可调用对象和参数绑定成为一个仿函数
namespace std_bind {
    using namespace std;

    void call_when_even(int x, const std::function<void(int)> &f) {
        if (!(x & 1)) {
            f(x);
        }
    }

    void output(int x) {
        cout << x << " ";
    }

    void output_add_2(int x) {
        cout << x + 2 << " ";
    }

    TEST(std_bind, case1) {
        {
            // placeholder表示该位置将被传入的第一个参数代替
            auto fr = bind(output, placeholders::_1);
            for (int i = 0; i < 10; ++i) {
                call_when_even(i, fr);
            }
            cout << endl;
        }
        {
            auto fr = bind(output_add_2, placeholders::_1);
            for (int i = 0; i < 10; ++i) {
                call_when_even(i, fr);
            }
            cout << endl;
        }
    }

    void output2(int x, int y) {
        cout << x << " " << y << endl;
    }

    // 灵活的bind
    TEST(std_bind, case2) {
       bind(output2, 1, 2)();
       bind(output2, placeholders::_1, 2)(1);
       bind(output2, 2, placeholders::_1)(1);
       bind(output2, 1, placeholders::_2)(1, 2);
       bind(output2, 2, placeholders::_2)(1, 2);
       bind(output2, placeholders::_1, placeholders::_2)(1, 2);
       bind(output2, placeholders::_2, placeholders::_1)(1, 2);
    }

    // std::bind + std::function
    class A {
    public:
        int i_ = 0;
        void output(int x, int y) {
            cout << x << " " << y << endl;
        }
    };

    TEST(std_bind, case3) {
        A a;
        function<void(int, int)> fr = bind(&A::output, &a, placeholders::_1, placeholders::_2);
        fr(1, 2);

        function<int&()> fr_i = bind(&A::i_, &a);
        fr_i() = 123;
        cout << a.i_ << endl;
    }
}