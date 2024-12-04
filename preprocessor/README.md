## Упрощенный препроцессор С++
Разработан препроцессор C++, который подставляет содержимое файлов на место `#include`-директив.
## Реализация
Разработана функция `Preprocess(...)`, которая в качестве параметров принимает путь к файлу `in_file` и рекурсивно подставляет все указанные `#include`-зависимости в выходной файл `out_file`, путь которого также передается в качестве аргумента в функцию. В функцию также передается вектор `include_directories`, который содержит директории для поиска.
Препроцессор обрабатывает два вида `#include`-директив:
- `#include "..."` - поиск файла выполняется относительно текущего файла, если файл не найден, поиск выполняется последовательно по всем элементам вектора `include_directories`.
- `#include <...>` - поиск выполняется последовательно по всем элементам вектора `include_directories`.

Для определения `#include`-директив используются регулярные вырожения, предоставляемые объектом класса `std::regex`:
- `#include "..."` задается регулярным выражением `\s*#\s*include\s*"([^"]*)"\s*`
- `#include <...>` задается регулярным выражением `\s*#\s*include\s*<([^>]*)>\s*`
  
Для проверки строки на соответствие регулярному выражению используется алгоритм `regex_match(...)`.

Если подключаемый файл так и не был найден или его не удалось открыть через `ifstream`, в поток `std::cout` выводится сообщение:
```
unknown include file <имя файла в директиве> at file <имя файла, где директива> at line <номер строки, где директива>
```
Для вывода `std::cout` сообщение об ошибки используется функция `PrintError(...)`, которая в качестве параметров принимает название файла `dest`, путь к файлу `src` и номер строки `string_number`:
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
