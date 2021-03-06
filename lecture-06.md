### Структуры

Данные структуры в памяти хранятся подряд.

### Указатели

Номера ячеек памяти, при этом указывается тип.

Операции:
* взятие адреса – `&a`
* разыменование – `*b`

### Массивы

Арифметика указателей: если указатель ссылается на какой-либо из элементов массива, то можно перейти к другим элементам.

```cpp
int a[10] = {10, 11, 12, 13, 14, 15, 16, 17, 18};
int* p = &a[5];
// *p == 15
// *(p - 1) == 14
// *(p + 2) == 17

p + 4;
4 + p;

p += 4;
p -= 4;
++p; --p;

p - q // – число элементов между ними, ptrdiff_t
```

Массив неявно конвертируется в указатель.

`int* b[10]` – массив указателей

`int (*b)[10]` – указатель на массив, обращение к нему `(*b)[10]`

Указатель на функцию – адрес первой инструкции.

Вместо `(*p).x` можно писать `p->x`.

`void *p = a` – указатель на `void` может хранить любой указатель.
