#include <iostream>
#include <cstddef>
#include <vector>
#include <random>
#include "gtest/gtest.h"
#include "set.h"

void assert_unique(const set<int> &s) {
    std::vector<int> c;
    for (int i : s) {
        EXPECT_TRUE(std::find(c.begin(), c.end(), i) == c.end());

        c.push_back(i);
    }
}

TEST(correctness, assign_op) {
    set<int> v;
    v.insert(1);
    v.insert(2);

    set<int> v2 = v;
    EXPECT_EQ(1, *v2.begin());
    EXPECT_EQ(2, *++v2.begin());

    v.clear();

    EXPECT_EQ(1, *v2.begin());
    EXPECT_EQ(2, *++v2.begin());
}

TEST(correctness, insert) {
    set<int> v;
    for (int i = 1; i < 1000; i++) {
        auto res = v.insert(i);
        EXPECT_TRUE(res.second);
    }

    auto it = v.begin();
    for (int i = 1; i < 1000; i++) {
        EXPECT_EQ(i, *it);
        ++it;
    }
    EXPECT_EQ(v.end(), it);

    assert_unique(v);
}

TEST(correctness, iterator_pointer_op) {
    set<std::string> v;
    v.insert("kek");
    (v.begin())->length();
}

TEST(correctness, clear_empty) {
    set<int> v;
    v.insert(1);
    v.insert(2);

    v.clear();
    ASSERT_TRUE(v.empty());
}

TEST(correctness, std_iterators) {
    set<int> v;
    v.insert(1);
    v.insert(2);
    v.insert(3);

    auto it = std::begin(v);
    EXPECT_EQ(1, *it);
    it = std::next(it);
    EXPECT_EQ(2, *it);
    it = std::prev(it);
    EXPECT_EQ(1, *it);
}

TEST(correctness, insert_nonunique) {
    set<int> v;
    v.insert(1);
    v.insert(2);
    v.insert(3);
    auto res = v.insert(3);
    EXPECT_FALSE(res.second);

    v.insert(-1);
    v.insert(-2);

    EXPECT_EQ(-2, *v.begin());
    EXPECT_EQ(-1, *++v.begin());
    EXPECT_EQ(1, *++ ++v.begin());

    assert_unique(v);
}

TEST(correctness, find) {
    std::vector<int> k;
    for (int i = 0; i < 1000; i++) k.push_back(i);

    std::shuffle(k.begin(), k.end(), std::default_random_engine());

    set<int> v;
    for (int i : k) {
        auto res = v.insert(i);
        EXPECT_TRUE(res.second);
    }

    for (int i = 0; i < 1000; i++) {
        EXPECT_NE(v.end(), v.find(i));
    }
    EXPECT_EQ(v.end(), v.find(2000));
    EXPECT_EQ(v.end(), v.find(-2000));
}

TEST(correctness, erase_) {
    set<int> s;
    s.insert(5);
    s.insert(2);
    s.insert(10);
    s.insert(6);
    s.insert(14);
    s.insert(7);
    s.insert(8);

    s.erase(s.find(5));
    EXPECT_EQ(2, *s.begin());
    EXPECT_EQ(6, *++s.begin());
    EXPECT_EQ(7, *++++s.begin());
    EXPECT_EQ(8, *++++++s.begin());
}

TEST(correctness, erase_insert) {
    set<int> s;
    s.insert(5);
    s.insert(2);
    s.insert(10);
    s.insert(6);
    s.insert(14);
    s.insert(7);
    s.insert(8);

    s.erase(s.find(2));
    EXPECT_EQ(5, *s.begin());
    EXPECT_EQ(6, *++s.begin());
    EXPECT_EQ(7, *++++s.begin());

    s.insert(2);
    EXPECT_EQ(2, *s.begin());
    EXPECT_EQ(5, *++s.begin());
    EXPECT_EQ(6, *++++s.begin());
    EXPECT_EQ(7, *++++++s.begin());
}

TEST(correctness, lower_bound) {
    std::vector<int> k;
    for (int i = 0; i < 1000; i += 2) k.push_back(i);

    std::shuffle(k.begin(), k.end(), std::default_random_engine());

    set<int> v;
    for (int i : k) {
        auto res = v.insert(i);
        EXPECT_TRUE(res.second);
    }

    for (int i = 0; i < 1000; i += 2) {
        EXPECT_EQ(i, *v.lower_bound(i));
    }
    EXPECT_EQ(0, *v.lower_bound(-1));
    EXPECT_EQ(0, *v.lower_bound(-500));
    EXPECT_EQ(2, *v.lower_bound(1));
    EXPECT_EQ(100, *v.lower_bound(99));
    EXPECT_EQ(v.end(), v.lower_bound(1001));
    EXPECT_EQ(v.end(), v.lower_bound(1000000));
}

TEST(correctness, lower_bound_empty) {
    set<int> v;
    v.lower_bound(-1);
}

TEST(correctness, upper_bound) {
    std::vector<int> k;
    for (int i = 0; i < 1000; i += 2) k.push_back(i);

    std::shuffle(k.begin(), k.end(), std::default_random_engine());

    set<int> v;
    for (int i : k) {
        auto res = v.insert(i);
        EXPECT_TRUE(res.second);
    }

    for (int i = 0; i < 1000 - 2; i += 2) {
        EXPECT_EQ(i + 2, *v.upper_bound(i));
    }
    EXPECT_EQ(0, *v.upper_bound(-1));
    EXPECT_EQ(0, *v.upper_bound(-500));
    EXPECT_EQ(100, *v.upper_bound(99));
    EXPECT_EQ(910, *v.upper_bound(909));
    EXPECT_EQ(v.end(), v.upper_bound(1001));
    EXPECT_EQ(v.end(), v.upper_bound(1000000));
}

TEST(correctness, upper_bound_end) {
    set<int> c;
    c.insert(1);
    c.insert(2);
    c.insert(3);
    c.insert(10);
    EXPECT_EQ(c.upper_bound(10), c.end());
}

TEST(correctness, upper_bound_end_) {
    set<int> c;
    c.insert(1);
    c.erase(c.begin());
    EXPECT_EQ(c.upper_bound(1), c.end());
}

TEST(correctness, upper_bound_) {
    set<int> c;
    c.insert(1);
    c.insert(2);
    c.insert(3);
    c.insert(10);
    EXPECT_EQ(c.upper_bound(3), --c.end());
}


TEST(correctness, empty) {
    set<int> s;
    EXPECT_TRUE(s.empty());

    s.insert(1);
    EXPECT_FALSE(s.empty());
}

TEST(correctness, swap) {
    set<int> s;
    s.insert(1);
    s.insert(3);
    s.insert(5);

    set<int> s2;
    s2.insert(2);
    s2.insert(4);

    swap(s, s2);
    EXPECT_EQ(2, *s.begin());
    EXPECT_EQ(4, *++s.begin());

    EXPECT_EQ(1, *s2.begin());

    assert_unique(s);
    assert_unique(s2);
}

TEST(correctness, swap_self) {
    set<int> s;
    s.insert(1);
    s.insert(3);

    swap(s, s);

    assert_unique(s);
    EXPECT_EQ(1, *s.begin());
    EXPECT_EQ(3, *++s.begin());
}

TEST(correctness, swap_empty) {
    set<int> s, s2;
    s.insert(1);
    s.insert(3);

    swap(s, s2);

    assert_unique(s2);
    EXPECT_EQ(1, *s2.begin());
    EXPECT_EQ(3, *++s2.begin());

    EXPECT_TRUE(s.empty());

    swap(s2, s);
    assert_unique(s);
    EXPECT_TRUE(s2.empty());
}

TEST(correctness, const_iterators) {
    set<int> s;
    s.insert(1);
    s.insert(2);
    const set<int>::iterator it = s.begin();
    EXPECT_EQ(1, *it);
}

struct dummy {
    int x;

    dummy() = delete;
    explicit dummy(int x) : x(x) {}

    friend bool operator<(const dummy &a, const dummy &b) { return a.x < b.x; }
    friend bool operator==(const dummy &a, const dummy &b) { return a.x == b.x; }
};

TEST(correctness, no_def_constructor) {
    set<dummy> s;
    s.insert(dummy(1));

    EXPECT_EQ(1, (*s.begin()).x);
}

struct throwing_new : dummy {
    explicit throwing_new(int x) : dummy(x) {}

    friend bool operator<(const throwing_new &a, const throwing_new &b) {
        throw std::runtime_error("lol");
    }
};

TEST(correctness, throwing_new) {
    set<throwing_new> s;
    s.insert(throwing_new(3));
    EXPECT_ANY_THROW(s.insert(throwing_new(1)));

    EXPECT_FALSE(s.empty());
}