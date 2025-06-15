# Optional<T> — Кастомная реализация std::optional на C++

`Optional<T>` — это собственная реализация контейнера опционального значения, аналогичная `std::optional` из стандарта C++17. Класс позволяет безопасно хранить объект типа `T`, который может быть как инициализирован, так и не инициализирован.

Реализация включает собственный тип исключения `BadOptionalAccess`, а также поддержку перемещения, копирования, сброса значения и безопасного доступа с проверкой.

---

## 🚀 Возможности

- Хранение значения типа `T` или пустого состояния
- Безопасный доступ к значению через `Value()`, `operator*`, `operator->`
- Бросает исключение `BadOptionalAccess` при попытке получить значение из пустого объекта
- Поддержка копирования и перемещения (включая `operator=` и конструкторы)
- Возможность сброса значения через `Reset()`
- Ручное управление жизненным циклом объекта через `placement new` и явный вызов деструктора

## 💡 Интерфейс

### Класс `Optional<T>`

```cpp
template <typename T>
class Optional {
public:
    Optional();
    Optional(const T& value);
    Optional(T&& value);
    Optional(const Optional& other);
    Optional(Optional&& other);

    Optional& operator=(const T& value);
    Optional& operator=(T&& rhs);
    Optional& operator=(const Optional& rhs);
    Optional& operator=(Optional&& rhs);

    ~Optional();

    bool HasValue() const;

    T& operator*();
    const T& operator*() const;
    T* operator->();
    const T* operator->() const;

    T& Value();             // бросает BadOptionalAccess, если значение отсутствует
    const T& Value() const;

    void Reset();

private:
    alignas(T) char data_[sizeof(T)];
    T* buff_ = nullptr;
    bool is_initialized_ = false;
};
```

## 📘 Пример использования

```cpp
#include "optional.h"
#include <iostream>
#include <string>

int main() {
    Optional<std::string> maybe_name;

    try {
        std::cout << maybe_name.Value();  // бросит BadOptionalAccess
    } catch (const BadOptionalAccess& ex) {
        std::cout << "Ошибка: " << ex.what() << '\n';
    }

    maybe_name = std::string("Alice");
    if (maybe_name.HasValue()) {
        std::cout << "Привет, " << *maybe_name << "!\n";
    }

    maybe_name.Reset();
    if (!maybe_name.HasValue()) {
        std::cout << "Имя сброшено\n";
    }
}
```



