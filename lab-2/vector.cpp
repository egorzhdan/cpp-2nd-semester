#include <cstdlib>
#include <new>
#include <cassert>
#include <iterator>
#include "vector.h"

template<typename T>
typename std::enable_if<std::is_trivially_copyable<T>::value>::type smart_copy(T *from, T *to, std::size_t cnt) {
    memcpy(to, from, cnt * sizeof(T));
}

template<typename T>
typename std::enable_if<!std::is_trivially_copyable<T>::value>::type smart_copy(T *from, T *to, std::size_t cnt) {
    for (std::size_t i = 0; i < cnt; i++) {
        to[i] = T(from[i]);
    }
}

template<typename T>
typename std::enable_if<std::is_trivially_destructible<T>::value>::type smart_delete(T *ptr, std::size_t cnt) {
    operator delete(ptr);
}

template<typename T>
typename std::enable_if<!std::is_trivially_destructible<T>::value>::type smart_delete(T *ptr, std::size_t cnt) {
    for (std::size_t i = 0; i < cnt; i++) {
        ptr[i].~T();
    }
    operator delete(ptr);
}

template<typename T>
T *vector<T>::alloc_storage(std::size_t capacity) {
    return (T *) operator new(sizeof(T) * capacity);
}

template<typename T>
vector<T>::vector() noexcept {
    _size = _capacity = 0;
    _storage = nullptr;
}

template<typename T>
vector<T>::vector(std::size_t initial_size) {
    _size = _capacity = initial_size;
    _storage = alloc_storage(initial_size);
    std::size_t i;
    try {
        for (i = 0; i < initial_size; i++) {
            new(&_storage[i]) T();
        }
    } catch (...) {
        for (std::size_t j = 0; j < i; j++) {
            _storage[j].~T();
        }
        throw;
    }
}

template<typename T>
vector<T>::vector(const vector<T> &other) {
    _size = _capacity = other._size;
    _storage = alloc_storage(_capacity);
    smart_copy(other._storage, _storage, _size);
}

template<typename T>
void vector<T>::increase_capacity() {
    assert(_size == _capacity);
    if (_capacity == 0) reserve(1);
    else reserve(_capacity * 2);
}

template<typename T>
void vector<T>::decrease_capacity() {
    assert(_size * 2 <= _capacity);
    if (_capacity == 1) return;

    auto *new_storage = alloc_storage(_capacity / 2);
    smart_copy(_storage, new_storage, _size);
    smart_delete(_storage, _capacity);

    _storage = new_storage;
    _capacity /= 2;
}

template<typename T>
void vector<T>::push_back(const T &item) {
    if (_size == _capacity) {
        increase_capacity();
    }
    new(&_storage[_size]) T(item);
    _size++;
}

template<typename T>
void vector<T>::pop_back() {
    _storage[--_size].~T();

    if (_size * 2 <= _capacity) decrease_capacity();
}

template<typename T>
std::size_t vector<T>::size() const {
    return _size;
}

template<typename T>
bool vector<T>::empty() const {
    return _size == 0;
}

template<typename T>
std::size_t vector<T>::capacity() const {
    return _capacity;
}

template<typename T>
void vector<T>::shrink_to_fit() {
    auto *new_storage = alloc_storage(_size);
    smart_copy(_storage, new_storage, _size);
    _storage = new_storage;
    _capacity = _size;
}

template<typename T>
T *vector<T>::data() {
    return _storage;
}

template<typename T>
const T *vector<T>::data() const {
    return _storage;
}

template<typename T>
T &vector<T>::back() {
    assert(_size > 0);
    return _storage[_size - 1];
}

template<typename T>
const T &vector<T>::back() const {
    assert(_size > 0);
    return _storage[_size - 1];
}

template<typename T>
T &vector<T>::operator[](std::size_t idx) {
    assert(idx < _size);
    return _storage[idx];
}

template<typename T>
const T &vector<T>::operator[](std::size_t idx) const {
    assert(idx < _size);
    return _storage[idx];
}

template<typename T>
void vector<T>::reserve(std::size_t new_capacity) {
    if (new_capacity <= _capacity) return;

    auto *new_storage = (T *) operator new(new_capacity * sizeof(T));
    smart_copy(_storage, new_storage, _size);
    smart_delete(_storage, _size);

    _storage = new_storage;
    _capacity = new_capacity;
}

template<typename T>
void vector<T>::resize(std::size_t new_size, T value) {
    reserve(new_size);

    for (std::size_t i = _size; i < new_size; i++) {
        _storage[i] = value;
    }
    _size = new_size;
}

template<typename T>
void vector<T>::clear() {
    operator delete(_storage);

    _storage = (T *) operator new(0);
    _size = _capacity = 0;
}

template<typename T>
typename vector<T>::iterator vector<T>::begin() {
    return empty() ? nullptr : _storage;
}

template<typename T>
typename vector<T>::iterator vector<T>::end() {
    return empty() ? nullptr : (_storage + _size);
}

template<typename T>
typename vector<T>::const_iterator vector<T>::begin() const {
    return empty() ? nullptr : _storage;
}

template<typename T>
typename vector<T>::const_iterator vector<T>::end() const {
    return empty() ? nullptr : (_storage + _size);
}

template<typename T>
vector<T> &vector<T>::operator=(const vector<T> &other) {
    _size = _capacity = other._size;
    _storage = alloc_storage(_size);
    smart_copy(other._storage, _storage, _size);
    return *this;
}

template<typename T>
typename vector<T>::reverse_iterator vector<T>::rbegin() {
    return reverse_iterator(end());
}

template<typename T>
typename vector<T>::reverse_iterator vector<T>::rend() {
    return reverse_iterator(begin());
}

template<typename T>
typename vector<T>::const_reverse_iterator vector<T>::rbegin() const {
    return const_reverse_iterator(end());
}

template<typename T>
typename vector<T>::const_reverse_iterator vector<T>::rend() const {
    return const_reverse_iterator(begin());
}

template<typename T>
template<typename Iterator>
vector<T>::vector(Iterator first, Iterator last) : vector<T>() {
    for (; first != last; first++) {
        push_back(*first);
    }
}

template<typename T>
template<typename Iterator>
void vector<T>::assign(Iterator first, Iterator last) {
    clear();
    for (; first != last; first++) {
        push_back(*first);
    }
}

template<typename T>
typename vector<T>::iterator vector<T>::insert(vector::const_iterator pos, const T &value) {
    std::size_t idx = pos - begin();

    if (_size == _capacity) increase_capacity();

    for (auto i = _size; i > idx; i--) {
        _storage[i] = _storage[i - 1];
    }
    _size++;
    _storage[idx] = value;

    return begin() + idx;
}

template<typename T>
void swap(vector<T> &a, vector<T> &b) {
    swap(a._storage, b._storage);
    swap(a._size, b._size);
    swap(a._capacity, b._capacity);
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(vector::const_iterator pos) {
    std::size_t idx = pos - begin();

    for (auto i = idx; i < _size - 1; i++) {
        _storage[i] = _storage[i + 1];
    }
    _size--;

    return begin() + idx;
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(vector::const_iterator from, vector::const_iterator to) {
    std::size_t from_idx = from - begin();
    std::size_t to_idx = to - begin();

    std::size_t diff = to_idx - from_idx;
    for (std::size_t i = from_idx; i < _size - diff; i++) {
        _storage[i] = _storage[i + diff];
    }
    _size -= diff;

    return begin() + from_idx;
}

template<typename T>
vector<T>::~vector() {
    smart_delete(_storage, _capacity);
}
