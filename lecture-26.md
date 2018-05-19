## C++-style casts

C: `(type) expr`

```cpp
struct base {
    virtual ~base();
};

struct derived : base {
    void g();
};

void f(base& obj) {
    if (typeid(obj) == typeid(derived)) {
        derived &d = (derived&) obj;
        d.g();
    }
}
```

C-style case позволяет привести что угодно к чему угодно, просто реинтерпретируя биты.

Каст между `base` и `derived` сдвигает указатель.

* `static_cast` (`int` <-> `int`, `base` <-> `derived`, `base*` <-> `derived*`, `base&` <-> `derived&`) – отсекает заведомо некорректные касты
* `reinterpret_cast` (`p*` <-> `int`) – implementation defined
* `const_cast` (`const int*` -> `int*`) – снимает `const`, запись не будет изменять объект или `segmentation fault` – адекватных использований почти нет
* `dynamic_cast` (`base` -> `derived`) – каст с проверкой: `if (derived* d = dynamic_cast<derived*>(b)) {}`
