## `test runner` - это утилита для юнит-тестирования
своего рода "мини-фреймворк", предназначенный для тестирования функций, проверки условий и вывода понятных сообщений об ошибках.

## Назначение
- Позволяет удобно сравнивать значения и проверять логические выражения в тестах.
- Автоматически печатать содержимое `vector`, `set`, `map`, `unordered_map` — для отладки и диагностики.
- Упрощает написание юнит-тестов с помощью макросов `ASSERT`, `ASSERT_EQUAL`, `RUN_TEST`
## Структура
1. Пространство имён `TestRunnerPrivate`
```C++
namespace TestRunnerPrivate {
  template <typename K, typename V, template <typename, typename> class Map>
  std::ostream& PrintMap(std::ostream& os, const Map<K, V>& m)
```
- Обобщённая функция для вывода `map`-подобных контейнеров (`std::map`, `std::unordered_map`).
- Используется в перегрузках `operator<<` ниже.
2. Перегрузка `operator<<` для стандартных контейнеров
Позволяет печатать `vector`, `set`, `map`, `unordered_map` прямо в поток вывода (`std::cout`, `std::cerr`), например:
```C++
std::vector<int> v = {1, 2, 3};
std::cout << v;  // {1, 2, 3}
```
3. `AssertEqual()` и `Assert()`
Путь к папке задается выражением следующего типа:
```C++
template <class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint = {})
```
- Сравнивает два значения и кидает исключение с пояснением, если они не равны.
- `Assert()` — вспомогательная обёртка, которая проверяет булевое выражение.
4. Класс TestRunner
```C++
class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const std::string& test_name);
```
- `RunTest()` — запускает тестовую функцию func и обрабатывает исключения, выводя результат.
- В деструкторе (`~TestRunner`) выводится сообщение об общем числе провалов и завершение с `exit(1)`, если тесты не прошли.
5. Макросы
`ASSERT_EQUAL(x, y)` проверяет, равны ли x и y. В случае ошибки показывает, что именно сравнивалось и где (`__FILE__`,` __LINE__`).
`ASSERT(x)` проверяет логическое выражение, аналогично `assert(x)` из ``C, но с более дружелюбным сообщением.
`RUN_TEST(tr, func)` запускает тест-функцию `func` через `TestRunner tr`.
## Пример использования
Пример 1 (тестирование структуры данных)
```С++
#include "test_runner.h"
#include <vector>

int Sum(const std::vector<int>& v) {
    int total = 0;
    for (int x : v) total += x;
    return total;
}

void TestSum() {
    ASSERT_EQUAL(Sum({1, 2, 3}), 6);
    ASSERT_EQUAL(Sum({}), 0);
    ASSERT_EQUAL(Sum({-1, -2, 3}), 0);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSum);
}
```
Пример 2 (тестирование функций)
```C++
#include "test_runner.h"
#include <string>

std::string Capitalize(const std::string& word) {
    if (word.empty()) return word;
    std::string result = word;
    result[0] = toupper(result[0]);
    return result;
}

void TestCapitalize() {
    ASSERT_EQUAL(Capitalize("hello"), "Hello");
    ASSERT_EQUAL(Capitalize("Hello"), "Hello");
    ASSERT_EQUAL(Capitalize(""), "");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCapitalize);
}

```
