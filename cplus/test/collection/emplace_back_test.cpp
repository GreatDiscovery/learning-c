//
// Created by Gavin on 2022/8/31.
//

#include "../basic.h"

struct A {
    int x;
    double y;

    A(int x, int y) : x(x), y(y) {}
};

struct Complicated {
    int year;
    double country;
    std::string name;

    Complicated(int year, double country, std::string name) :
            year(year), country(country), name(name) {
        std::cout << "is constructed" << std::endl;
    }

    Complicated(const Complicated &other) : year(other.year), country(other.country), name(std::move(other.name)) {
        std::cout << "is moved" << std::endl;
    }
};

TEST(emplace_test, emplace_back减少内存拷贝和移动) {
    std::vector<A> vector;
    // 这里通过函数传参直接生成了一个A
    vector.emplace_back(1, 2);
    std::cout << vector.size() << std::endl;
}

TEST(emplace_test, 比push_back更具性能优势) {
    // todo
}