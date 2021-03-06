Автогенерируемые функции:

```cpp
struct foo {
    // конструктор:
    // foo() {}
    
    // конструктор копирования:
    // foo(foo const&);
    
    // деструктор:
    // ~foo() {}
}
```

* `foo a;` – вызывается конструктор по-умолчанию
* `foo a(1, 2, 3)` – вызывается конструктор `foo(int, int, int)`
* `foo c(b)` – вызывается конструктор копирования

Оператор присваивания не эквивалентен копированию.

Иногда не хочется, чтобы эти функции генерировались:

```cpp
struct string {
    char* data;
    size_t size;
    
    // сгенерировался деструктор
}

string a = "abc";
string b = a; // фейл
```

а для такого кода встроенные функции будут работать хорошо:

```cpp
struct BigInt {
    vector<uint32_t> data;
    bool negative;
}
```

Если не хотим, чтобы кто-то использовал эти функции, можно написать так:

```cpp
struct foo {
    foo() {}
    
private:
    foo(foo const&);
    
    foo& operator=(foo const&);
    
    ~foo();
}
```

В C++11 появился способ запретить перегрузки – когда компилятор в них попадает, происходит ошибка компиляции.

```cpp
// ...
foo& operator=(foo const&) = delete;
// ...
```

Восстановить генерацию:

```cpp
// ...
foo() = default;
// ...
```

### Способы создать объект:
* `foo a;` / `foo b(1, 2)`
* `f(foo(1, 2, 3))`
* `foo *p = new foo(1, 2, 3)` – создаем объект и затем сами удаляем – `delete p`. 
Операции `new`/`delete` достаточно дорогие. Есть специальная форма для массивов: `foo* p = new foo[3]` / `delete[] p` – так никогда не пишем, используем `vector`
* `malloc` / `free` – их используют редко, только в низкоуровневом коде, потому что обычно нужна инициализация

Пока что считаем, `new`/`delete` – это единственный способ создать объект с произвольным временем жизни.
Но в современном C++ `new`/`delete` не стоит использовать.  