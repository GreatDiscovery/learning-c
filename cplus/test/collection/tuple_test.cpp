//
// Created by Gavin on 2022/10/8.
//

#include "../basic.h"
using namespace std;

std::tuple<double, char, std::string> get_student(int id)
{
    if (id == 0) return std::make_tuple(3.8, 'A', "Lisa Simpson");
    if (id == 1) return std::make_tuple(2.9, 'C', "Milhouse Van Houten");
    if (id == 2) return std::make_tuple(1.7, 'D', "Ralph Wiggum");
    throw std::invalid_argument("id");
}

// 类模板 std::tuple 是固定大小的异类值汇集。它是 std::pair 的推广
TEST(tuple_test, 熟悉tuple的用法) {
    auto student0 = get_student(0);
    std::cout << "ID: 0, "
              << "GPA: " << std::get<0>(student0) << ", "
              << "grade: " << std::get<1>(student0) << ", "
              << "name: " << std::get<2>(student0) << '\n';

    double gpa1;
    char grade1;
    std::string name1;
    std::tie(gpa1, grade1, name1) = get_student(1);
    std::cout << "ID: 1, "
              << "GPA: " << gpa1 << ", "
              << "grade: " << grade1 << ", "
              << "name: " << name1 << '\n';

    // C++17 结构化绑定：
    auto [ gpa2, grade2, name2 ] = get_student(2);
    std::cout << "ID: 2, "
              << "GPA: " << gpa2 << ", "
              << "grade: " << grade2 << ", "
              << "name: " << name2 << '\n';
}

template <class T>
void tuple_size_test(T t)
{
    int a[std::tuple_size<T>::value]; // 可在编译时
    std::cout << std::tuple_size<T>::value << '\n'; // 或在运行时使用
}

TEST(tuple_test, 熟悉tuple的一些函数) {
    tuple_size_test(std::make_tuple(3.8, 'A', "Lisa Simpson"));
    std::tuple<int> tp1 = std::make_tuple(1);
    std::tuple<int, double> tp2 = std::make_tuple(1, 2.0);
    cout << std::get<0>(tp1) << std::endl;
    cout << std::get<1>(tp2) << std::endl;
}