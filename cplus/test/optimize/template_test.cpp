//
// Created by Gavin on 2022/9/24.
//

#include "../basic.h"

using namespace std;
template <class... T>
void f (T... args) {
    cout << sizeof...(args) << endl;
}

void print() {
    cout << "empty!" << endl;
}
template <class T, class... Args>
void print(T head, Args... rest) {
    cout << "parameter " << head << endl;
    print(rest...);
}

template<std::size_t I = 0, typename Tuple>
typename std::enable_if<I == std::tuple_size<Tuple>::value>::type printtp(Tuple t) {
    cout << "empty!" << endl;
}

// 参数模板和typename和可变参数结合起来太难理解了，需要继续看源码。尝试理解源码是如何做的
template<std::size_t I = 0, typename Tuple>
typename std::enable_if<I < std::tuple_size<Tuple>::value>::type printtp(Tuple t) {
    cout << std::get<I>(t) << std::endl;
    printtp<I + 1>(t);
}

template<typename... Args>
void printByTuple(Args... args) {
    printtp(std::make_tuple(args...));
}

TEST(template_test, 可变参数模板) {
    f();
    f(1, 2);
    f(1, 2.5, "");
}


TEST(template_test, 递归方式展开参数包) {
    print(1, "2", 3.0, 4);
}

TEST(template_test, 通过type_traits展开参数包) {
    printByTuple("1", "2", 3, 4.0);
}