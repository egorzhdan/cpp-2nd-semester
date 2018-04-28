```cpp
struct base {
    virtual void print() {
        cout << "base\n";
    }
};

struct derived : base {
    void print() {
        cout << "derived\n";
    }
};

int main() {
    derived d;
    base& b = d;
    d.print();
}
```
 – выведет `derived`, потому что функция виртуальная.
 
 А если напишем:
 
```cpp
int main() {
    derived d;
    base& b = d;
    d.print();
}
```
 – выведет `base`, потому что создался новый объект типа `base`.
 
Это называется **slicing**. Почти всегда это нежелательно делать. Он иногда возникает не только при присваивании,
но и при касте или приеме аргументов у функции.


# Исключения

Методы работы с ОС могут вернуть ошибку, и ее не всегда получится отследить. 

```cpp
void do_something() {
    FILE* file = fopen("kek.txt"); // вернет nullptr, если файл не открылся
    if (!file)
        return false;
    
    size_t bytes = fread(..., file);
    if (bytes < 0) return false;
    
    bytes = fread(..., file);
    if (bytes < 0) return false;
    
    // ...
}

bool h() {
    // много лишнего кода, если хотим вызвать do_something
}
```

Функция сообщает, успешно ли завершилось. Это использование кодов возврата.

Чтобы было меньше мусорного кода, сделали исключения.

```cpp
void f() {
    // ...
    throw runtime_error("f failed");
}

int main() {
    try {
        // ...
        g();
    }
    catch (runtime_error const& e) {
        // ...
    }
}
```

Можно бросать исключения произвольного типа, а ловить базового. 
Если ни один из `catch` блоков не подошел, то исключение полетит дальше. 
Проверяться будут сверху вниз.

Бросать можно любой (почти) некопируемый тип.

Есть специальная форма catch, чтобы ловить любой exception: `catch (...)`. 
Если хочется сделать операцию, а потом продолжить изначальный exception, можно написать так:
```cpp
catch (...) {
    ...
    throw;
}
```

```cpp
struct base {
    virtual string msg() const {
        return "base";
    }
}

struct derived : base {
    string msg() const {
        return "derived";
    }
}

int main() {
    try {
        // ...
    }
    catch (base const& e) {
        // ...
    }
}
```

Просто `throw` бросает исключение статического типа, если так не хочется, напишем `throw e1`.

`throw` всегда скопирует объект.

### Best practices

В исключении можно хранить данные, связанные с ошибкой.

Примеры использования:
 * запускаем вычисление, которое хотим уметь останавливать, `cancellation_exception`
 * пользователь решил отменить действие
 
Общее у этих примеров – если получив исключение, вызывающая сторона может захотеть пробросить наверх.

Время работы ухудшается, когда летит исключение, но обычно всем на это пофиг.