//
// Created by Gavin on 2022/7/31.
//

#include "iterator_test.h"
#include "iostream"
#include "gtest/gtest.h"

using namespace std;

// 自定义实现一个迭代器
namespace detail_range {
    template<typename T>
    class iterator {
    public:
        using value_type = T;
        using size_type = size_t;
    private:
        size_t cursor_;
        const value_type step_;
        value_type value_;

    public:
        iterator(size_type cur_start, value_type begin_value, value_type step_val) : cursor_(cur_start),
                                                                                     step_(step_val),
                                                                                     value_(begin_value) {
            value_ += (step_ * cursor_);
        }

        value_type operator*() const {
            return value_;
        }

        bool operator!=(const iterator &rhs) const {
            return (cursor_ != rhs.cursor_);
        }

        iterator &operator++(void) {
            value_ += step_;
            ++cursor_;
            return (*this);

        }
    };

    template<typename T>
    class impl {
    public:
        using value_type = T;
        using reference = const value_type &;
        using const_reference = const value_type &;
        using iterator = const detail_range::iterator<value_type>;
        using const_iterator = const detail_range::iterator<value_type>;
//        typename（类型名字）关键字告诉编译器把一个特殊的名字解释成一个类型
        using size_type = typename iterator::size_type;

    private:
        const value_type begin_;
        const value_type end_;
        const value_type step_;
        const size_type max_count_;

        size_type get_adjusted_count() const {
            if (step_ > 0 && begin_ >= end_) {
                throw std::logic_error("End value must be greater than begin value.");
            } else if (step_ < 0 && begin_ <= end_) {
                throw std::logic_error("End value must be less than begin value.");
            }

            size_type x = static_cast<size_type>((end_ - begin_) / step_);
            if (begin_ + (step_ * x) != end_) {
                ++x;
            }
            return x;
        }

    public:
        impl(value_type begin_vlaue, value_type end_val, value_type step_value) : begin_(begin_vlaue), end_(end_val),
                                                                                  step_(step_value),
                                                                                  max_count_(get_adjusted_count()) {}

        size_t size() const {
            return max_count_;
        }

        const_iterator begin() const {
            return {0, begin_, step_};
        }

        const_iterator end() const {
            return {max_count_, begin_, step_};
        }
    };

//    reference wrapper 是一个引用包裹器，可以包裹一个指向对象或者指向函数指针的引用，既可以通过拷贝构造，也可以通过赋值构造；
    template<typename T>
    impl<T> range(T end) {
        return {{}, end, 1};
    }

    template<typename T>
    impl<T> range(T begin, T end) {
        return {begin, end, 1};
    }

//    decltype与auto关键字一样，用于进行编译时类型推导,以一个普通表达式作为参数，返回该表达式的类型,而且decltype并不会对表达式进行求值
// 这里的返回类型加了自动推导，所以写法有些复杂，参考：https://www.cnblogs.com/ishen/p/13190179.html
    template<typename T, typename U>
    auto range(T begin, T end, U step) -> detail_range::impl<decltype(begin + step)> {
        using r_t = impl<decltype(begin + step)>;
        return r_t(begin, end, step);
    }

    TEST(custom_iterator, case1) {
        cout << "range(15):";
        for (int i : range(15)) {
            cout << " " << i;
        }
        cout << endl;
    }

    TEST(custom_iterator, case2) {
        cout << "range(2, 6):";
        for (int i : range(2, 6)) {
            cout << " " << i;
        }
        cout << endl;
    }

    TEST(custom_iterator, case3) {
        cout << "range(-2, -6, -3):";
        for (int i : range(-2, -6, -3)) {
            cout << " " << i;
        }
        cout << endl;
    }

    TEST(custom_iterator, case4) {
        cout << "range('a', 'z'):";
        for (int i : range('a', 'z')) {
            cout << " " << i;
        }
        cout << endl;
    }
}

