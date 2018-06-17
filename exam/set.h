#ifndef SET_H
#define SET_H

#include <algorithm>
#include <cassert>
#include <iterator>
#include <utility>

template <typename T>
struct set {
    set() noexcept {}
    set(const set<T> &other) {
        try {
            for (const auto &it : other) {
                insert(it);
            }
        } catch (...) {
            clear();
            throw;
        }
    }
    set &operator=(const set &other) {
        set<T> tmp(other);
        swap(tmp);
        return *this;
    }
    ~set() {
        clear();
    }

    bool empty() const noexcept { return !root.left; }
    void clear() noexcept {
        assert(!root.right);
        if (root.left) {
            root.left->clean();
            delete root.left;
            root.left = nullptr;
        }
    }

    void swap(set<T> &other) noexcept {
        if (root.left && other.root.left) {
            std::swap(root.left->parent, other.root.left->parent);
            std::swap(root.left, other.root.left);
        } else if (root.left) {
            other.root.left = root.left;
            other.root.left->parent = &other.root;
            root.left = nullptr;
        } else if (other.root.left) {
            root.left = other.root.left;
            root.left->parent = &root;
            other.root.left = nullptr;
        }
    }

private:
    template <typename U>
    struct value_node;

    template <typename U>
    struct node {
        node<U> *parent;
        value_node<U> *left;
        value_node<U> *right;

        node() : parent(nullptr), left(nullptr), right(nullptr) {}
        node(value_node<U> *left, value_node<U> *right) : parent(nullptr), left(left), right(right) {}
        node(node<U> *parent, value_node<U> *left, value_node<U> *right) : parent(parent), left(left), right(right) {}

        node<U> *leftmost_child() {
            if (left) return left->leftmost_child();
            return (value_node<U> *) this;
        }
        node<U> *rightmost_child() {
            if (right) return right->rightmost_child();
            return this;
        }

        node<U> *next() {
            if (right) return right->leftmost_child();

            auto *cur = parent;
            auto *prev = this;
            while (cur && prev == cur->right) {
                prev = cur;
                cur = cur->parent;
            }
            return cur;
        }
        node<U> *prev() {
            if (left) return left->rightmost_child();

            auto *cur = parent;
            auto *prev = this;
            while (cur && prev == cur->left) {
                prev = cur;
                cur = cur->parent;
            }
            return cur;
        }
    };

    template <typename U>
    struct value_node : node<U> {
        U value;

        explicit value_node(U value) : value(value) {}

        void clean() noexcept {
            if (this->left) {
                this->left->clean();
                delete this->left;
            }
            if (this->right) {
                this->right->clean();
                delete this->right;
            }
        }

        value_node<U> *lower_bound(const T &item) {
            if (value == item) return this;

            if (item > value) {
                if (this->right)
                    return this->right->lower_bound(item);
                else return nullptr;
            } else {
                value_node<U> *res = nullptr;
                if (this->left) res = this->left->lower_bound(item);
                return res ? res : this;
            }
        }

        value_node<U> *maybeNext() {
            auto *raw = this->next();
            return raw ? ((value_node<U> *) raw) : nullptr;
        }

        value_node<U> *upper_bound(const T &item) {
            auto *lb = lower_bound(item);
            if (lb && (lb->value == item)) lb = lb->maybeNext();
            return lb;
        }

        void swap(value_node<U> *other) {
            value_node<U> lhs = *this;
            value_node<U> rhs = *other;

            if (lhs.parent) {
                if (lhs.parent->left == this)
                    lhs.parent->left = other;
                else
                    lhs.parent->right = other;
            }
            if (rhs.parent) {
                if (rhs.parent->left == other)
                    rhs.parent->left = this;
                else
                    rhs.parent->right = this;
            }

            if (lhs.left) lhs.left->parent = other;
            if (lhs.right) lhs.right->parent = other;
            if (rhs.left) rhs.left->parent = this;
            if (rhs.right) rhs.right->parent = this;

            std::swap(this->parent, other->parent);
            std::swap(this->right, other->right);
            std::swap(this->left, other->left);
        }

        value_node<U> *detach() {
            if (this->left && this->right) {
                auto *nl = (value_node<U> *) this->left->rightmost_child();
                swap(nl);
                return detach();
            }

            if (!this->right) {
                if (this->parent) {
                    if (this == this->parent->right)
                        this->parent->right = this->left;
                    else
                        this->parent->left = this->left;
                }
                if (this->left)
                    this->left->parent = this->parent;
            } else {
                if (this->parent) {
                    if (this == this->parent->right)
                        this->parent->right = this->right;
                    else
                        this->parent->left = this->right;
                }
                this->right->parent = this->parent;
            }

            return this;
        }
    };

    template <typename U>
    struct basic_iterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = U;
        using pointer = U *;
        using reference = U &;

        node<U> *cur;

        basic_iterator() : basic_iterator(nullptr) {}
        basic_iterator(const basic_iterator<U> &other) = default;
        explicit basic_iterator(node<U> *n) : cur(n) {}

        reference operator*() const { return ((value_node<U> *) cur)->value; }
        pointer operator->() const { return &((value_node<U> *) cur)->value; }

        basic_iterator &operator++() {
            auto vn = (value_node<U> *) cur;
            if (!vn) return *this;
            cur = vn->next();
            return *this;
        }
        basic_iterator operator++(int) {
            basic_iterator prev(*this);
            ++*this;
            return prev;
        }

        basic_iterator &operator--() {
            auto vn = (value_node<U> *) cur;
            if (!vn) return *this;
            cur = vn->prev();
            return *this;
        }
        basic_iterator operator--(int) {
            basic_iterator prev(*this);
            --*this;
            return prev;
        }

        friend bool operator==(basic_iterator const &lhs, basic_iterator const &rhs) { return lhs.cur == rhs.cur; }
        friend bool operator!=(basic_iterator const &lhs, basic_iterator const &rhs) { return lhs.cur != rhs.cur; }

        friend bool operator==(basic_iterator const &lhs, std::reverse_iterator<basic_iterator> const &rhs) { return lhs.cur == rhs.base().cur; }
    };

    mutable node<const T> root;

public:
    using const_iterator = basic_iterator<const T>;
    using iterator = const_iterator;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using reverse_iterator = const_reverse_iterator;

    const_iterator begin() const {
        auto l = root.leftmost_child();
        return const_iterator(l);
    }
    const_iterator end() const {
        return const_iterator(&root);
    }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    std::pair<const_iterator, bool> insert(const T &item) {
        value_node<const T> *cur = root.left;
        if (!cur) {
            root.left = new value_node<const T>(item);
            root.left->parent = &root;
            return {const_iterator(root.left), true};
        }

        while (cur) {
            if (item < cur->value) {
                if (cur->left) cur = cur->left;
                else {
                    cur->left = new value_node<const T>(item);
                    cur->left->parent = cur;
                    return {const_iterator(cur->left), true};
                }
            } else if (cur->value < item) {
                if (cur->right) cur = cur->right;
                else {
                    cur->right = new value_node<const T>(item);
                    cur->right->parent = cur;
                    return {const_iterator(cur->right), true};
                }
            } else return {const_iterator(cur), false};
        }

        throw std::runtime_error("invalid state");
    }

    const_iterator erase(const_iterator ite) {
        const_iterator next(ite);
        next++;

        auto *rm = ((value_node<T> *) ite.cur)->detach();
        if (rm) delete rm;

        return next;
    }

    const_iterator find(const T &item) const {
        auto *cur = root.left;
        while (cur) {
            if (cur->value == item) { return const_iterator(cur); }

            if (item < cur->value) {
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return end();
    }

    const_iterator lower_bound(const T &item) const {
        auto *res = (root.left) ? root.left->lower_bound(item) : nullptr;
        return res ? const_iterator(res) : end();
    }
    const_iterator upper_bound(const T &item) const {
        auto *res = (root.left) ? root.left->upper_bound(item) : nullptr;
        return res ? const_iterator(res) : end();
    }
};

template <typename T>
void swap(set<T> &left, set<T> &right) {
    left.swap(right);
}

#endif //SET_H
