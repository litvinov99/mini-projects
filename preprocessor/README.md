## Препроцессор
Разработан препроцессор C++, который подставляет содержимое файлов на место `#include`-директив.
Разработана функция `Preprocess(in_file, out_file, include_directories)`, которая принимает на вход путь к файлу `in_file` и рекурсивно подставляет все `#include`-зависимости. Результат подстановки записывается в `out_file`.

Функция обрабатывает два вида `#include`-директив:
- `#include "..."` - поиск файла выполняется относительно текущего файла, где расположена сама директива. Если файл не найден, поиск выполняется последовательно по всем элементам вектора `include_directories`.
- `#include <...>` - поиск выполняется последовательно по всем элементам вектора `include_directories`.  
Если включаемый файл так и не был найден или его не удалось открыть через `ifstream`, в поток std::cout выводится сообщение:
```
unknown include file <имя файла в директиве> at file <имя файла, где директива> at line <номер строки, где директива>
```
## Реализация
Регулярное выражение представляется объектом класса `std::regex`. Для проверки строки на соответствие регулярному выражению используется алгоритм `regex_match(...)`.
- `#include "..."` задается регулярным выражением `\s*#\s*include\s*"([^"]*)"\s*`
- `#include <...>` задается регулярным выражением `\s*#\s*include\s*<([^>]*)>\s*`
Для вывода std::cout сообщение об ошибки используется функция `PrintError(...)`, которая в качестве параметров принимает название файла `dest`, путь к файлу `src` и номер строки `string_number`:
```c++
void PrintError(const string& dest, const string& src, int string_number) {
    cout << "unknown include file "s << dest
         << " at file " << src << " at line "s << string_number << endl;
}
```
Подстановку содержимого файлов на место `#include`-директив и, в случае ошибки, вызов функции `PrintError(...)` выполняет функция `SubstitutionOfDirectives(...)`, которая в качестве параметров принимает входной `input` и выходной `output` потоки, путь к файлу для подстановки `in_file` и вектор директорий для поиска `include_directories`.
## Пример работы
Пусть дана следующая структура файлов и каталогов:
- `sources/a.cpp`
```c++
  // this comment before include
  #include "dir1/b.h"
  // text between b.h and c.h
  #include "dir1/d.h"

  void SayHello() {
      std::cout << "hello, world!" << std::endl;
  #   include<dummy.txt>
  }  
```
- `sources/dir1/b.h`
```c++
  // text from b.h before include
  #include "subdir/c.h"
  // text from b.h after include 
```
- `sources/dir1/subdir/c.h`
```c++
  // text from c.h before include
  #include <std1.h>
  // text from c.h after include
```
- `sources/dir1/d.h`
```c++
  // text from d.h before include
  #include "lib/std2.h"
  // text from d.h after include
```
- `sources/include1/std1.h`
```c++
  // text from d.h before include
  #include "lib/std2.h"
  // text from d.h after include
```
- `sources/include2/lib/std2.h`
```c++
  // std2
```
В качестве `include`-директорий поданы `sources/include1` и `sources/include2`, тогда программа создаст такой файл:
```c++
// this comment before include
// text from b.h before include
// text from c.h before include
// std1
// text from c.h after include
// text from b.h after include
// text between b.h and c.h
// text from d.h before include
// std2
// text from d.h after include

void SayHello() {
    std::cout << "hello, world!" << std::endl;
```
В `std::cout` будет выведен текст:
```c++
unknown include file dummy.txt at file sources/a.cpp at line 8 
```
