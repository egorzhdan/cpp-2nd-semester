### Copy-on-write

При копировании строки буфер скопируется. 
Но можно не копировать его, а продолжить ссылаться на исходный.

Рядом с буфером хранится счетчик ссылок. 
Если он равен 1, то можем просто менять строку.
Если больше, то нужно скопировать буфер.

В `fbstring` есть три состояния: small, обычный, copy-on-write.

```cpp
struct dynamic_storage {
    size_t ref_counter;
    size_t capacity;
    char data[]; // данные занимают место за границей структуры
                 // может быть только последним членом структуры
};

dynamic_storage new_dynamic_storage(size_t capacity) {
    dynamic_storage* result = (dynamic_storage*) malloc(sizeof(dynamic_storage) + sizeof(char) * capacity);
    
    result->ref_counter = 1;
    result->capacity = capacity;
    
    return result;
}

struct string {
    size_t size;
    union {
        dynamic_storage* data;
        char inplace_storage[sizeof(void*)];
    };
};
```

## Оператор `new`

1. `new T(1, 2, 3)` / `delete p` – new expression
1. `malloc(100)` / `free(p)`
1. constructor / destructor
1. `void* p = operator new(100)` / `operator delete(p)` – operator new, вызов обычной функции

`p = new T(1, 2, 3)` эквивалентна `p = operator new(sizeof(T)); /* constructor() */`, 
но если они могут бросать исключения, то такой код небезопасный. Так лучше:
```cpp
p = operator new(sizeof(T));
try {
    /* constructor() */
} catch (...) {
    operator delete(p);
    throw;
}
```

Реализация:
```cpp
void* operator new(size_t size) {
    void* res = malloc(size);
    if (!res) throw std::bad_alloc();
    return res;
}
```

На самом деле синтаксис оператора `new` другой: `new (1, 2, 3) T(4, 5, 6)`. 
Аргументы `(1, 2, 3)` передаются оператору `new`, который можно переопределить
Аналогично работает `delete`.

Поэтому нужно писать `delete` с такой же сигнатурой, как `new`.

Можно писать `new(std::nothrow) T(1, 2, 3)`. 
Оператор `new` от `nothrow` не бросает исключения, когда не удалось выделить память.

Третья форма: `new(size_t size, void* location)` позволяет вызвать конструктор на уже выделенной памяти.
Это называется _placement new_. 

```cpp
int main() {
    void *p = malloc(sizeof(std::string));
    // ...
    std::string* s = new (p) std::string("Hello");
    s->append("x");
    s->~std::string();
}
```

Так работает `std::vector`. Элементы между `size` и `capacity` – это еще не инициализированная память.
Когда нужно ее поиспользовать, он создает элемент таким образом.

В C++11 стало можно писать следующее:
```cpp
union {
    std::string x;
    std::vector<char> y;
};
```
Неясно, какой конструктор вызвать. На деле нужно вызывать самому.

