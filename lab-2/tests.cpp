#include <iostream>
#include <cstddef>
#include "gtest/gtest.h"
#include "vector.h"
#include "vector.cpp"

TEST(basic, push_pop_size_back) {
    vector<int> v;
    for (int i = 1; i < 10000; i++) {
        v.push_back(i);
        ASSERT_EQ(i, v.size());
        ASSERT_EQ(i, v.back());
    }
    for (int i = 9998; i >= 0; i--) {
        v.pop_back();
        ASSERT_EQ(i, v.size());
    }
}

TEST(basic, init_resize) {
    vector<int> v(100);
    ASSERT_EQ(100, v.size());
    for (std::size_t i = 0; i < 100; i++) {
        v[i] = 1;
    }
    ASSERT_EQ(100, v.size());

    v.resize(500);
    ASSERT_EQ(500, v.size());

    ASSERT_EQ(0, v.back());
    v.resize(10);
    ASSERT_EQ(10, v.size());
}

TEST(basic, init_copy) {
    vector<int> v(100);
    ASSERT_EQ(100, v.size());
    for (std::size_t i = 0; i < 100; i++) {
        v[i] = 1;
    }

    vector<int> v2(v);
    for (std::size_t i = 0; i < 100; i++) {
        ASSERT_EQ(v[i], v2[i]);
    }

    v[0] = 123;
    ASSERT_EQ(v2[0], 1);
}

TEST(basic, init_equal) {
    vector<int> v(100);
    ASSERT_EQ(100, v.size());
    for (std::size_t i = 0; i < 100; i++) {
        v[i] = 1;
    }

    vector<int> v2 = v;
    for (std::size_t i = 0; i < 100; i++) {
        ASSERT_EQ(v[i], v2[i]);
    }

    v[0] = 123;
    ASSERT_EQ(v2[0], 1);
}

TEST(basic, init_reserve) {
    vector<int> v(10);
    ASSERT_EQ(10, v.size());

    v.reserve(100);
    ASSERT_EQ(100, v.capacity());
}

TEST(basic, init_resize_clear) {
    vector<int> v(20);
    ASSERT_EQ(20, v.size());

    v.resize(50);
    ASSERT_EQ(50, v.size());

    v.clear();
    ASSERT_TRUE(v.empty());
    ASSERT_EQ(0, v.size());

    v.resize(100);
    ASSERT_EQ(100, v.size());
}

TEST(basic, shrink) {
    vector<int> v(100);
    ASSERT_EQ(100, v.capacity());

    v.reserve(1000);
    ASSERT_EQ(1000, v.capacity());
    v.shrink_to_fit();
    ASSERT_EQ(100, v.capacity());
}

TEST(iterators, begin_end) {
    vector<int> v(20);
    for (std::size_t i = 0; i < v.size(); i++) {
        v[i] = (int) i;
    }

    int i = 0;
    for (auto it : v) {
        ASSERT_EQ(it, i);
        i++;
    }
}

TEST(iterators, begin_end_const) {
    const vector<int> v(20);

    for (const auto &it : v) {
        ASSERT_EQ(it, 0);
    }
}

TEST(iterators, rbegin_rend) {
    vector<int> v(20);
    for (std::size_t i = 0; i < v.size(); i++) {
        v[i] = (int) i;
    }

    int i = 19;
    for (auto it = v.rbegin(); it != v.rend(); it++) {
        ASSERT_EQ(i, *it);
        i--;
    }
}

TEST(iterators, init_iter) {
    int a[] = {0, 1, 2, 3, 4};

    vector<int> v(a, a + 5);

    int i = 0;
    for (const auto &it : v) {
        ASSERT_EQ(it, i);
        i++;
    }
}

TEST(iterators, insert_erase) {
    vector<int> v(100);
    for (std::size_t i = 0; i < v.size(); i++) {
        v[i] = (int) i;
    }

    v.insert(v.begin(), 228);
    ASSERT_EQ(101, v.size());
    ASSERT_EQ(228, v[0]);

    v.insert(v.begin() + 10, 566);
    ASSERT_EQ(566, v[10]);

    v.erase(v.begin());
    ASSERT_EQ(0, v[0]);

    v.erase(v.begin() + 1);
    ASSERT_EQ(2, v[1]);

    v.erase(v.begin(), v.end());
    ASSERT_TRUE(v.empty());
}

TEST(no_sfinae, strings) {
    vector<std::string> v1;
    v1.push_back("abc");
    v1.push_back("qwe");

    vector<std::string> v2 = v1;
    ASSERT_EQ(2, v2.size());

    v1.pop_back();
    ASSERT_EQ(2, v2.size());
    ASSERT_EQ(1, v1.size());

    v2.push_back("ddd");
    ASSERT_EQ(3, v2.size());
    ASSERT_EQ(1, v1.size());
}

struct throws {
    throws() { throw std::runtime_error(""); }
};

TEST(basic, init_throws) {
    ASSERT_THROW(vector<throws> v(10), std::runtime_error);
}