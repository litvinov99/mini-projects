## test runner - это утилита для юнит-тестирования
своего рода "мини-фреймворк", предназначенный для тестирования функций, проверки условий и вывода понятных сообщений об ошибках.

## Структура и назначение
*Позволяет удобно сравнивать значения и проверять логические выражения в тестах.
*Автоматически печатать содержимое vector, set, map, unordered_map — для отладки и диагностики.
*Упрощает написание юнит-тестов с помощью макросов ASSERT, ASSERT_EQUAL, RUN_TEST
1. Пространство имён TestRunnerPrivate
```C++
namespace TestRunnerPrivate {
  template <typename K, typename V, template <typename, typename> class Map>
  std::ostream& PrintMap(std::ostream& os, const Map<K, V>& m)
```
*Обобщённая функция для вывода map-подобных контейнеров (std::map, std::unordered_map).
*Используется в перегрузках operator<< ниже.
2. Перегрузка operator<< для стандартных контейнеров
Позволяет печатать vector, set, map, unordered_map прямо в поток вывода (std::cout, std::cerr), например:
```C++
std::vector<int> v = {1, 2, 3};
std::cout << v;  // {1, 2, 3}
```
3. AssertEqual() и Assert()
Путь к папке задается выражением следующего типа:
```C++
template <class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint = {})
```
*Сравнивает два значения и кидает исключение с пояснением, если они не равны.
*Assert() — вспомогательная обёртка, которая проверяет булевое выражение.
4. Класс TestRunner
```C++
class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const std::string& test_name);
```
*RunTest() — запускает тестовую функцию func и обрабатывает исключения, выводя результат.
*В деструкторе (~TestRunner) выводится сообщение об общем числе провалов и завершение с exit(1), если тесты не прошли.
5. Макросы
ASSERT_EQUAL(x, y)
Проверяет, равны ли x и y. В случае ошибки показывает, что именно сравнивалось и где (__FILE__, __LINE__).
ASSERT(x)
Проверяет логическое выражение, аналогично assert(x) из C, но с более дружелюбным сообщением.
RUN_TEST(tr, func)
Запускает тест-функцию func через TestRunner tr.
## Пример использования
```С++
void TestSum() {
  ASSERT_EQUAL(2 + 2, 4);
  ASSERT(5 > 3);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSum);
}
```
