```cpp
template <typename U, typename V>
struct mytype<pair<U, V>> {};
```

Специализация может иметь меньше параметров.

Иногда хочется вместо параметра передать значение. Например, как в `std::array<int, 10>`.
```cpp
template <typename T, size_t N>
struct array {
    // ...
};
```
Так можно делать с любыми целочисленными типами и `enum`.

Можно специализировать: `struct array<T, 5>`.

Можно сделать параметры темплейта по-умолчанию:
```cpp
template <typename T, typename A = std::allocator<T>>
struct vector {
    // ...
};
```
Начиная с C++11 можно делать переменное число темплейтных аргументов.

Пусть есть типы `A`, `B`, `C`, и у каждого функция `print`.
Функция `print_100_times` хочет быть полиморфной по аргументу.
Можно сделать общий базовый класс.
Иначе:
```cpp
template <typename T>
void print_100_times(T const& abc) {
    // ...
    print(abc);
}
```

```cpp
template <typename Logger>
void do_x(Logger&) {}; // – статический полиморфизм

// или

void do_x(logger& log) {}; // – динамический полиморфизм
```


