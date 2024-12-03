#include <cassert>
#include <cstring>
#include <iostream>
#include <string>

#include "my_string.h"

void Test1() {
    {
        MyString str;
        assert(str.lenght() == 0);
    }

    {
       MyString str(5, 'a');
       assert(str == "aaaaa");
       assert(str.lenght() == 5);
    }

    {
       MyString str("Test string");
       assert(str == "Test string");
       assert(str.lenght() == strlen("Test string"));
    }
    std::cout << "Test 1 is OKAY\n";
}

int main() {
    Test1();
    std::string test1 = "223";
    int num = std::stoi(test1);
    MyString str("Test string");
}