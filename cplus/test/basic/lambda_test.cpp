//
// Created by Gavin on 2022/8/13.
//

// lambda： [捕获列表] (params) opt -> return_type { body; };
#include "../basic.h"

using namespace std;

class A {
public:
    A(int input) : i(input) {}

public:
    int i = 0;

    void func(int x, int y) {
//            auto x1 = [] {return i;};
        // 捕获所有外部变量，按值传递
        auto x2 = [=] { return i + x + y; };
        // 捕获所有外部变量，按引用传递
        auto x3 = [&] {
            i = 1;
            return i + x + y;
        };
        auto x4 = [this] { return i; };
        auto x5 = [this, x, y] { return i + x + y; };
        // this也是指针，会改变i的值
        auto x7 = [this] { return i++; };
        // mutable，可以修改按值捕获的外部变量
        auto x8 = [=]() mutable { return i++; };
        cout << x2() << endl;
        cout << x3() << endl;
        cout << x4() << endl;
        cout << x5() << endl;
        cout << x7() << " i=" << i << endl;
        cout << x8() << " i=" << i << endl;

    }
};

class CountEven {
    int &count;

public:
    CountEven(int &count) : count(count) {}

    void operator()(int val) {
        if (!(val & 1)) {
            ++count;
        }
    }
};

TEST(lambda_test, 捕获变量) {
    A a(1);
    a.func(1, 1);
}

TEST(lambda_test, lambda替换仿函数) {
    vector<int> v = {1, 2, 3, 4, 5, 6};
    int even_count = 0;
    for_each(v.begin(), v.end(), CountEven(even_count));
    cout << "the number of even is " << even_count << endl;

    even_count = 0;
    for_each(v.begin(), v.end(), [&even_count](int val) {
        if (!(val & 1)) {
            ++even_count;
        }
    });
    cout << "the number of even is " << even_count << endl;
}

TEST(lambda_test, lambda可以替换function) {
    vector<int> v = {1, 2, 3, 4, 5, 6};
    // 查找大于2的个数
    // 1. bind方式
    using placeholders::_1;
    auto f = std::bind(logical_and<>(), std::bind(greater<int>(), _1, 1), std::bind(less_equal<int>(), _1, 6));
    cout << count_if(v.begin(), v.end(), f) << endl;
    // 2. lambda方式
    int count = count_if(v.begin(), v.end(), [](int x) {
        return x > 2;
    });
    cout << count << endl;
}