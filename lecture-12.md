### Множественное наследование

```cpp
struct base1 {
    int a, b;
};

struct base2 {
    int a, c, d;
};

struct derived: base1, base2 {
    // ...
};

derived d;
d.a //ambiguous access of 'a'
d.base1::a  // ok
```

Пример 1.
```cpp
struct x {
	int a;
};

struct y : x {
	int b;
};

struct z : x{
	int c;
};

struct w : y, z {
	// ...
};

w ww;
ww.a; //CE - в w две копии a
```
Пример 2.
```cpp
struct x {
	int a;
};

struct y : virtual x {
	int b;
};

struct z : virtual x{
	int c;
};

struct w : y, z {
	// ...
};

w ww;
ww.a; //ok, в w один sub-object типа x

```
С функциями будет так же: без virtual не компилится, а с virtual - ok. А что будет если функцию сделать чисто витуальной?

Пример 3.
```cpp
struct x {
	virtual void f() = 0;
};

struct y : virtual x {
	int b;
};

struct z :  virtual x {
	int c;
};

struct w : y, z {
	// ...
};

...
w ww; //попытка создать объект абстрактного класса
```
Между этими двумя примерами был случай, когда он дописал f в y и z, и ему удалось скомплировать из-за наличия лишнего virtual ????

Пример 4. 
```cpp
struct x {
	virtual void f() = 0;
};

struct y : virtual x {
	void f() {};
	int b;
};

struct z : virtual x {
	void f() {};
	int c;
};

struct w :  y, z {
	// ...
};

...
w ww; 
/* CE - no unique final overrider for f. Если оставить f только в одном месте либо в y, либо в z, то все будет ok*/

// w.f() //не интересно
``` 
Пример 5. 
```cpp
struct x {
	virtual void f() = 0;
};

struct y : x {
	void f() {};
	int b;
};

struct z : x {
	int c;
};

struct w :  y, z {
	// ...
};

...
w ww; 
/* В w две копии f, одна из них не заоверрайдена. Ошибка как в примере 3*/
``` 


`override` – ключевое слово, если не повторяет сигнатуру родительской функции, то ошибка компиляции

`final` – функция не может быть переопределена

`mix-in` – способ собирать объекты из частей.