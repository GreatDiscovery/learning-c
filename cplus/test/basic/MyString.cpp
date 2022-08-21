//
// Created by Gavin on 2022/8/21.
//

#include "../basic.h"

class MyString {
private:
    char *m_data;
    size_t m_len;
    void copy_data(const char *s) {
        m_data = new char[m_len + 1];
        memcpy(m_data, s, m_len);
        m_data[m_len] = '\0';
    }

public:
    MyString() {
        m_data = NULL;
        m_len = 0;
    }

    MyString(const char *p) {
        m_len = strlen(p);
        copy_data(p);
    }

    MyString(const MyString &str) {
        m_len = str.m_len;
        copy_data(str.m_data);
        std::cout << "copy constructor!" << std::endl;
    }

    MyString& operator=(const MyString &str) {
        if (this != &str) {
            m_len = str.m_len;
            copy_data(str.m_data);
        }
        std::cout << "copy assign!" << std::endl;
        return *this;
    }

    virtual ~MyString() {
        if (m_data) {
            std::cout << "delete[] m_data:" << m_data << std::endl;
            // 调用整个数组的析构函数，而不是只是删除第一个
            delete[] m_data;
        }
    }
};

TEST(string_test, 测试MyString) {
    MyString str;
    str = MyString("Hello");
    std::vector<MyString> vector;
    vector.push_back(str);
}