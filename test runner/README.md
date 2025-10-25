# C++ Unit Test Framework

## Оглавление

- [Описание](#описание)
- [Особенности реализации](#особенности-реализации)
- [Требования](#требования)
- [Использование](#использование)
- [API](#api)
- [Примеры](#примеры)

## Описание

Профессиональный фреймворк для unit-тестирования на C++. Предоставляет минималистичный и эффективный набор инструментов для написания и запуска тестов с детализированными сообщениями об ошибках и поддержкой стандартных контейнеров STL.

## Особенности реализации

### Архитектурные особенности

- **Шаблонные assertion-функции**: Поддержка любых типов с оператором `==`
- **Расширенная диагностика**: Автоматический вывод содержимого контейнеров STL при ошибках
- **Изоляция пространств имен**: Вспомогательные функции в `TestRunnerPrivate`
- **RAII управление**: Автоматическая проверка результатов в деструкторе TestRunner

### Поддерживаемые контейнеры

Фреймворк автоматически поддерживает вывод:

- `std::vector<T>`
- `std::set<T>`
- `std::map<K, V>
- `std::unordered_map<K, V>`

### Обработка ошибок
```cpp
// Детализированные сообщения об ошибках с указанием файла и строки
"Assertion failed: 5 != 10, test_math.cpp:42"
```

## Требования

### Системные требования

- Компилятор C++11 или новее
- Стандартная библиотека STL
- Поддерживаемые платформы: Linux, Windows, macOS
### Зависимости

- **STL**: `vector`, `map`, `set`, `unordered_map`, `string`, `sstream`
- **Исключения**: `std::runtime_error`

## Использование

### Базовое использование
```cpp
#include "test_runner.h"

void TestAddition() {
    ASSERT_EQUAL(2 + 2, 4);
    ASSERT_EQUAL(5 + 3, 8);
}

void TestVector() {
    std::vector<int> v = {1, 2, 3};
    std::vector<int> expected = {1, 2, 3};
    ASSERT_EQUAL(v, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestAddition);
    RUN_TEST(tr, TestVector);
    return 0;
}
```

### Расширенное использование
```cpp
#include "test_runner.h"
#include <map>
#include <set>

void TestContainers() {
    // Тестирование std::map
    std::map<int, std::string> m = {{1, "one"}, {2, "two"}};
    std::map<int, std::string> expected_map = {{1, "one"}, {2, "two"}};
    ASSERT_EQUAL(m, expected_map);
    
    // Тестирование std::set
    std::set<int> s = {1, 2, 3, 4, 5};
    std::set<int> expected_set = {1, 2, 3, 4, 5};
    ASSERT_EQUAL(s, expected_set);
}

void TestWithCustomHint() {
    int actual = CalculateSomething();
    int expected = 42;
    
    // Кастомное сообщение об ошибке
    if (actual != expected) {
        AssertEqual(actual, expected, "Calculation failed with input: " + std::to_string(input));
    }
}
```

## API

### Основные функции

#### AssertEqual
```cpp
template <class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint = {});
```
Проверяет равенство двух значений. При неудаче генерирует исключение с детализированным сообщением.

**Параметры:**
- `t`: Первое значение для сравнения
- `u`: Второе значение для сравнения
- `hint`: Дополнительное сообщение об ошибке
#### Assert
```cpp
void Assert(bool b, const std::string& hint);
```
Проверяет истинность условия. Удобная обертка для булевых проверок.

**Параметры:**

- `b`: Проверяемое условие
- `hint`: Сообщение об ошибке

### Класс TestRunner
#### RunTest
```cpp
template <class TestFunc>
void RunTest(TestFunc func, const std::string& test_name);
```
Запускает тестовую функцию и обрабатывает исключения.

**Параметры:**
- `func`: Тестовая функция (лямбда или обычная функция)
- `test_name`: Имя теста для вывода в консоль

### Макросы

#### ASSERT_EQUAL
```cpp
ASSERT_EQUAL(actual, expected);
```
Макрос для проверки равенства с автоматическим определением файла и строки.

#### ASSERT
```cpp
ASSERT(condition);
```
Макрос для проверки условия с автоматическим определением файла и строки.

#### RUN_TEST
```
RUN_TEST(test_runner, test_function);
```
Макрос для запуска теста через TestRunner.

## Примеры

### Тестирование математических операций
```cpp
#include "test_runner.h"
#include <cmath>

void TestMathOperations() {
    ASSERT_EQUAL(5 * 5, 25);
    ASSERT_EQUAL(10 / 2, 5);
    ASSERT_EQUAL(std::pow(2, 3), 8);
    ASSERT_EQUAL(std::sqrt(16), 4);
    
    // Проверка с плавающей точкой (требует осторожности)
    ASSERT(std::abs(0.1 + 0.2 - 0.3) < 1e-10);
}
```

### Тестирование строковых операций
```cpp
#include "test_runner.h"
#include <string>

void TestStringOperations() {
    std::string str = "Hello";
    ASSERT_EQUAL(str + " World", "Hello World");
    ASSERT_EQUAL(str.size(), 5);
    ASSERT_EQUAL(str.substr(0, 2), "He");
    
    ASSERT(str.find('e') != std::string::npos);
}
```

### Тестирование пользовательских типов
```cpp
#include "test_runner.h"

struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x << ", " << p.y << ")";
}

void TestCustomType() {
    Point p1{1, 2};
    Point p2{1, 2};
    Point p3{3, 4};
    
    ASSERT_EQUAL(p1, p2);
    // ASSERT_EQUAL(p1, p3); // Будет ошибка: (1, 2) != (3, 4)
}
```

### Интеграция с существующим проектом
```cpp
#include "test_runner.h"
#include "my_library.h"

void TestLibraryFunctionality() {
    MyLibrary lib;
    
    // Тестирование нормальных случаев
    ASSERT_EQUAL(lib.Calculate(5), 25);
    ASSERT_EQUAL(lib.Process("input"), "processed_input");
    
    // Тестирование пограничных случаев
    ASSERT_EQUAL(lib.Calculate(0), 0);
    ASSERT_EQUAL(lib.Process(""), "processed_");
    
    // Тестирование контейнеров
    auto result = lib.GetData();
    std::vector<int> expected = {1, 2, 3, 4, 5};
    ASSERT_EQUAL(result, expected);
}

int main() {
    TestRunner tr;
    
    RUN_TEST(tr, TestLibraryFunctionality);
    // Добавление других тестов...
    
    return 0;
}
```
