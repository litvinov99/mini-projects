#include <exception>
#include <iostream>
#include <cassert>
#include <string>

#include "ScopedPtr.h"

int main() {

    using namespace std;
    // Проверка работы оператора приведения к типу bool
    {
        // Для нулевого указателя приведение к типу bool возвращает false
        const ScopedPtr<int> empty_ptr;
        assert(!empty_ptr);

        // Для ненулевого указателя приведение к типу bool возвращает true
        const ScopedPtr<int> ptr_to_existing_object(new int(0));
        assert(ptr_to_existing_object);

        static_assert(noexcept(static_cast<bool>(ptr_to_existing_object)));
    }

    // Проверка работы оператора разыменования *
    {
        string* raw_ptr = new string("hello");
        ScopedPtr<string> smart_ptr(raw_ptr);
        // Ссылка, возвращаемая оператором разыменования, должна ссылаться на объект,
        // на который указывает умный указатель
        assert(&*smart_ptr == raw_ptr);

        try {
            ScopedPtr<int> empty_ptr;
            // При попытке разыменовать пустой указатель должно быть выброшено
            // исключение logic_error
            *empty_ptr;
            // Сюда попасть мы не должны
            assert(false);
        } catch (const logic_error&) {
            cout << "Ошибка поймана" << endl;
            // мы там, где нужно
        } catch (...) {
            // Других исключений выбрасываться не должно
            assert(false);
        }
    }

    // Проверка работы оператора ->
    {
        string* raw_ptr = new string("hello");
        ScopedPtr<string> smart_ptr(raw_ptr);
        // Доступ к членам класса через умный указатель должен быть аналогичен
        // доступу через "сырой" указатель
        assert(smart_ptr->data() == raw_ptr->data());

        try {
            ScopedPtr<string> empty_ptr;
            // При попытке разыменовать пустой указатель должно быть выброшено
            // исключение logic_error
            empty_ptr->clear();
            // Сюда попасть мы не должны
            assert(false);
        } catch (const logic_error&) {
            // мы там, где нужно
        } catch (...) {
            // Других исключений выбрасываться не должно
            assert(false);
        }
    }

    // Пример использования
    {
        // На этой структуре будет проверяться работа умного указателя
        struct Object {
            Object() {
                cout << "Object is default constructed"s << endl;
            }
            void DoSomething() {
                cout << "Doing something"s << endl;
            }
            ~Object() {
                cout << "Object is destroyed"s << endl;
            }
        };

        // Сконструированный по умолчанию указатель ссылается на nullptr
        ScopedPtr<Object> empty_smart_ptr;
        // Перегруженный оператор приведения к типу bool вернёт false для пустого указателя
        assert(!empty_smart_ptr);

        ScopedPtr<Object> smart_ptr(new Object());
        // Перегруженный оператор bool вернёт true для указателя, ссылающегося на объект
        assert(smart_ptr);

        // Проверка оператора разыменования
        (*smart_ptr).DoSomething();
        // Проверка оператора доступа к членам класса
        smart_ptr->DoSomething();
    }
} 