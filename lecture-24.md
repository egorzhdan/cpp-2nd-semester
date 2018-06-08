### Итераторы

```cpp
erase(iterator);

insert(iterator, T const& it);

erase(iterator, iterator);
```

`std::reverse_iterator` позволяет итерироваться в обратном порядке. 
У него есть метод `.base()`, который возвращает обычный итератор.


### Vector

```cpp
void push_back(T a) { // compile error
    if (...) {
        a.f();
    } else {
        a.g(); 
    }
}
```
– не компилируется, если у класса нет обоих методов.

Можно ассоциировать свойства классов аналогично `numeric_limits` – **traits**.

**Tag dispatching**:
```cpp
template <typename T>
struct tag {}

template <typename T>
void print_impl(T t, tag<false>) { /* ... */ }

template <typename T>
void print_impl(T t, tag<true>) { /* ... */ }
```

Правило **SFINAE** – substitution failure is not an error:
```cpp
template <typename C> // C is int[10]

void f(C& c, typename C::iterator i);

void f(E (&c)[N] E* i);

int main() {
    int a[10];
    f(a, a + 5);
} 
```

```cpp
template <bool V>
struct enable_if;

template<>
struct enable_if<true> {
    typedef void type;
}

typename enable_if<std::is_array<T>::value>::type print(T const& v) {
    cout << v[0];
}

typename enable_if<!std::is_array<T>::value>::type print(T const& v) {
    cout << v;
}
```
– такой класс есть в std.

