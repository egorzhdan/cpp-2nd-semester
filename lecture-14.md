В C++ нет `finally`, вместо него используется деструктор. Это называется RAII _(resource allocation is initialization)_.

**Списки инициализации**

```cpp
struct config_file {
    config_file()
        : f1("1.txt")
        , f2("2.txt")
        
    file_descriptor f1, f2;
};
```

Если в теле конструкции будет исключение, то все поля будут уничтожены (в порядке объявления).

```cpp
int main() {
    int *a = new int(5);
    int *b = new int(5);
    
    delete b;
    delete a;
}
```
– проблема такая же, как с файлами – удаление не произойдет, если `new` зафейлится.

Поэтому нужно использовать готовые обертки – `vector` и потоки IO.

```cpp
struct unique_ptr {
    unique_ptr() : p(nullptr) { }
    
    unique_ptr(int* p) : p(p) { }
    
    unique_ptr(unique_ptr const&) = delete;
    unique_ptr& operator=(unique_ptr const&) = delete;
    
    ~unique_ptr() {
        delete p;
    }
    
private:
    int* p;
};

int main() {
    unique_ptr a(new int(5));
    unique_ptr b(new int(6));
}
```

Это называется smart pointer.

Можно написать `template <typename T>` и все `int` заменить на `T`.

Такой класс `unique_ptr` есть в стандартной библиотеке.

Неявное приведение к `unique_ptr` может приводить к тому, что `delete` вызовется несколько раз.

Ключевое слово `explicit` для конструктора – запретить неявное приведение на конструкторе. 
Каждый раз стоит думать, нужно ли писать `explicit`.

Для оператора присваивания имеет смысл сделать сначала копию, затем `swap`. Это называется swap trick –
можно из любой операции с базовой гарантией исключений получить функцию со строгой гарантией.
