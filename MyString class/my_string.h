#pragma once

#include <iostream>

class MyString {
private:
    char* str;
    // size_t len;

public:
    MyString();
    MyString(const char* other);
    MyString(size_t count, char ch);
    ~MyString();

    MyString(const MyString& other);
    MyString& operator=(const MyString& other);

    MyString& operator+=(const MyString& rhs);


    void swap(MyString& rhs);    
    size_t lenght();
    void print() const;

};

std::ostream& operator<<(std::ostream& out, const MyString& string);
