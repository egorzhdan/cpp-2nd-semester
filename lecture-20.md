# Тестирование

1. Статический анализ: coverity, clang static analyser, etc
1. Динамический анализ: sanitizer (address, memory, thread, undefined behaviour, etc), valgrind

Динамический находит ошибки только в той части кода, которая выполнилась.
Чтобы все было быстрее, можно писать тесты для отдельных функций.

Чтобы понять, какой код был протестирован, используют тесты и code coverage.

**Fuzzing** tests – будем случайно генерировать входные данные и запускать. 
Если какая-то из внутренних проверок в программе зафейлилась, то ошибка найдена.

**Coverage guided fuzzing** – fuzzing, который зависит от изменения coverage при каждом прогоне тестов.
Новые тесты генерируются мутацией подобранных тестов.

**Symbolic execution**: klee
