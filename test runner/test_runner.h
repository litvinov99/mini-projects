#pragma once

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace TestRunnerPrivate {
  /// @brief Вспомогательная функция для печати ассоциативных контейнеров
  /// @tparam K Тип ключа
  /// @tparam V Тип значения
  /// @tparam Map Тип контейнера (map, unordered_map)
  /// @param os Выходной поток
  /// @param m Контейнер для печати
  /// @return Ссылка на выходной поток
  template <typename K, typename V, template <typename, typename> class Map>
  std::ostream& PrintMap(std::ostream& os, const Map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
      if (!first) {
        os << ", ";
      }
      first = false;
      os << kv.first << ": " << kv.second;
    }
    return os << "}";
  }
}

/// @brief Перегрузка оператора вывода для std::vector
/// @tparam T Тип элементов вектора
/// @param os Выходной поток
/// @param s Вектор для печати
/// @return Ссылка на выходной поток
template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

/// @brief Перегрузка оператора вывода для std::set
/// @tparam T Тип элементов множества
/// @param os Выходной поток
/// @param s Множество для печати
/// @return Ссылка на выходной поток
template <class T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

/// @brief Перегрузка оператора вывода для std::map
/// @tparam K Тип ключа
/// @tparam V Тип значения
/// @param os Выходной поток
/// @param m Словарь для печати
/// @return Ссылка на выходной поток
template <class K, class V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m) {
  return TestRunnerPrivate::PrintMap(os, m);
}

/// @brief Перегрузка оператора вывода для std::unordered_map
/// @tparam K Тип ключа
/// @tparam V Тип значения
/// @param os Выходной поток
/// @param m Хэш-таблица для печати
/// @return Ссылка на выходной поток
template <class K, class V>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<K, V>& m) {
  return TestRunnerPrivate::PrintMap(os, m);
}

/// @brief Проверяет равенство двух значений
/// @tparam T Тип первого значения
/// @tparam U Тип второго значения
/// @param t Первое значение
/// @param u Второе значение
/// @param hint Сообщение об ошибке (опционально)
/// @throws std::runtime_error Если значения не равны
template <class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint = {}) {
  if (!(t == u)) {
    std::ostringstream os;
    os << "Assertion failed: " << t << " != " << u;
    if (!hint.empty()) {
      os << " hint: " << hint;
    }
    throw std::runtime_error(os.str());
  }
}

/// @brief Проверяет истинность условия
/// @param b Проверяемое условие
/// @param hint Сообщение об ошибке
/// @throws std::runtime_error Если условие ложно
inline void Assert(bool b, const std::string& hint) {
  AssertEqual(b, true, hint);
}

/// @brief Класс для запуска и управления unit-тестами
class TestRunner {
public:
  /// @brief Запускает тестовую функцию
  /// @tparam TestFunc Тип тестовой функции
  /// @param func Тестовая функция
  /// @param test_name Имя теста для вывода
  template <class TestFunc>
  void RunTest(TestFunc func, const std::string& test_name) {
    try {
      func();
      std::cerr << test_name << " OK" << std::endl;
    } catch (std::exception& e) {
      ++fail_count;
      std::cerr << test_name << " fail: " << e.what() << std::endl;
    } catch (...) {
      ++fail_count;
      std::cerr << "Unknown exception caught" << std::endl;
    }
  }

  /// @brief Деструктор - проверяет результаты всех тестов
  ~TestRunner() {
    std::cerr.flush();
    if (fail_count > 0) {
      std::cerr << fail_count << " unit tests failed. Terminate" << std::endl;
      exit(1);
    }
  }

private:
  int fail_count = 0;  ///< Счетчик неудачных тестов
};

#ifndef FILE_NAME
#define FILE_NAME __FILE__
#endif

/// @brief Макрос для проверки равенства значений с информацией о месте ошибки
/// @param x Первое значение
/// @param y Второе значение
#define ASSERT_EQUAL(x, y)                                               \
  {                                                                      \
    std::ostringstream __assert_equal_private_os;                        \
    __assert_equal_private_os << #x << " != " << #y << ", " << FILE_NAME \
                              << ":" << __LINE__;                        \
    AssertEqual(x, y, __assert_equal_private_os.str());                  \
  }

/// @brief Макрос для проверки истинности условия с информацией о месте ошибки
/// @param x Проверяемое условие
#define ASSERT(x)                                                  \
  {                                                                \
    std::ostringstream __assert_private_os;                        \
    __assert_private_os << #x << " is false, " << FILE_NAME << ":" \
                        << __LINE__;                               \
    Assert(x, __assert_private_os.str());                          \
  }

/// @brief Макрос для запуска теста через TestRunner
/// @param tr Экземпляр TestRunner
/// @param func Тестовая функция
#define RUN_TEST(tr, func) tr.RunTest(func, #func)
