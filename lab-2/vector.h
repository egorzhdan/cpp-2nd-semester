#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <cstddef>

template<typename T>
struct vector {
public:
    vector() noexcept;

    explicit vector(std::size_t initial_size);

    vector(const vector<T> &other);

    vector &operator=(const vector<T> &other);

    void push_back(const T &item);

    void pop_back();

    T &back();

    const T &back() const;

    T &operator[](std::size_t idx);

    const T &operator[](std::size_t idx) const;

    std::size_t size() const;

    bool empty() const;

    T *data();

    const T *data() const;

    void reserve(std::size_t new_capacity);

    std::size_t capacity() const;

    void shrink_to_fit();

    void clear();

    void resize(std::size_t new_size, T value = T());

    ~vector();

    // Iterators:

    typedef T *iterator;
    typedef const T *const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    iterator begin();

    const_iterator begin() const;

    iterator end();

    const_iterator end() const;

    reverse_iterator rbegin();

    const_reverse_iterator rbegin() const;

    reverse_iterator rend();

    const_reverse_iterator rend() const;

    iterator insert(const_iterator pos, const T &value);

    iterator erase(const_iterator pos);

    iterator erase(const_iterator from, const_iterator to);

    template<typename Iterator>
    vector(Iterator first, Iterator last);

    template<typename Iterator>
    void assign(Iterator first, Iterator last);

    friend void swap(vector<T> &a, vector<T> &b);

private:
    std::size_t _size;
    std::size_t _capacity;
    T *_storage;

    void increase_capacity();
    void decrease_capacity();

    T *alloc_storage(std::size_t capacity);
};

#endif //VECTOR_VECTOR_H
