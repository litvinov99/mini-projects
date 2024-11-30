#include <iostream>
#include <string>

#include "my_string.h"

int main() {

    MyString str1 = "Hello";
    MyString str2 = " World!";
    str1 += str2;
    str1.print();

}