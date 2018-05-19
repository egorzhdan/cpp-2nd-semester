# Namespace

```cpp
namespace gnome {
    namespace panel {
        // ...
    }
}
```

```cpp
using namespace gnome::panel; // (1) using directive
```

Лучше не использовать `using namespace` в header.

```cpp
using std::vector;            // (2) using declaration
```

Разница в том, где возникнет ошибка при использовании пересекающихся namespace.

Чтобы исправить, можно переименовать один из классов, но `vector` шаблонный, поэтому `typedef` использовать нельзя:
```cpp
template <typename T>
using myvector = std::vector<T>; // type alias
```

Анонимный namespace:
```cpp
namespace {
    // ...
}
```
– как если бы компилятор дал какое-то имя, а затем вставил `using namespace`.

```cpp
struct base {
    void f();
}

struct derived : base {
    void f(int);
}

derived().f(5); // works
derived().f(); // compile error
```
решается так:
```cpp
struct derived : base {
    using base::f;
    void f(int);
}
```

Еще через `using` можно сделать публичными `protected`-методы из базы.

### ADL (argument-dependent lookup)
(Koenig lookup)

```cpp
namespace n {
    struct big_int { /* ... */ }
    
    operator+(big_int const& a, big_int const& b) { /* ... */ }
}

int main() {
    n::big_int a, b;
    // ...
    a + b; // вызывается operator+, который лежит в namespace n
           // и не будет находиться линковщиком
}
```
Чтобы такого не было, правила поменяли, и на самом деле находится.

ADL срабатывает только для неквалифицированных вызовов (для `swap` работает, для `std::swap` нет).
Но если мы не знаем тип, то часто пишут `using std::swap; swap(a, b)` – будет работать ADL.
