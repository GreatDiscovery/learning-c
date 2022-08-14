//
// Created by Gavin on 2022/8/14.
//

#include "../basic.h"
#include "stdio.h"
// tuple可以替代结构体
using namespace std;

template<typename Tuple, std::size_t N>
struct tuple_printer
{
    static void print(const Tuple& t)
    {
        tuple_printer<Tuple, N - 1>::print(t);
        std::cout << ", " << std::get<N - 1>(t);
    }
};

template<typename Tuple>
struct tuple_printer<Tuple, 1>
{
    static void print(const Tuple& t)
    {
        std::cout << std::get<0>(t);
    }
};

template<typename... Args>
void print_tuple(const std::tuple<Args...>& t)
{
    tuple_printer<decltype(t), sizeof...(Args)>::print(t);
    std::cout << std::endl;
}

TEST(tuple_test, 创建tuple) {
    std::tuple<int, std::string, float> t1(10, "TEST", 3.14);
    int n = 7;
    auto t2 = std::tuple_cat(t1, std::make_pair("FOO", "bar"), t1, std::tie(n));
    n = 10;
    print_tuple(t2);
}