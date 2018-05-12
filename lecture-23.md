# STL

– часть стандартной библиотеки.

Если у нас есть ассоциативная операция (`a * (b * c) = (a * b) * c`) и массив (`a[]`),
то можно к нему применить parallel reduce.

1. containers: sequences (`vector`, `list`, `deque`), associative (regex: `[unordered][multi][set/map]`)
1. iterators: каждый поддерживает операции (`*it`, `it == other`, `it != other`, `it++`, `++it`), каждый container поддерживает `begin()` и `end()`; 
бывают видов `Input`, `Output`, `Forward`, `Bidirectional` (`--it`, `it--`), `RandomAccess` (`it += n`, `it -= n`, `it <=> other`, `it - other`)
1. algorithms (`find`, `sort`)
1. functional objects – то, что можно вызвать (например, указатель на функцию, или переопределенный оператор `()` у структуры)
1. container adapters (`stack`, `queue`, `priority_queue`)

Указатели являются итераторами, то есть для `int a[10]` – `*a` и `*(a + 10)`

У многих контейнеров есть свои правила. Например, у `vector` требуется деструктор и для некоторых операций требуется конструктор без аргументов.
