#pragma once

#include <iostream>


class MyString {
private:
    char* buffer_ = nullptr;
    size_t len_ = 0;

public:
    //============= DECLARATIONS =================
    using Iterator = char*;
    using ConstIterator = const char*;

    //============= CTORS & DTOR =================
    MyString() = default;
    MyString(const char* other);
    MyString(size_t count, char ch);
    ~MyString();

    MyString(const MyString& other);
    MyString(MyString&& other);

    //=============== OPERATORS ==================
    MyString& operator=(const MyString& other) noexcept;
    MyString& operator=(MyString&& other) noexcept;

    char& operator[](size_t index) noexcept;
    const char& operator[](size_t index) const noexcept;

    MyString& operator+=(const MyString& rhs);
    bool operator!=(const MyString& rhs) const;

    //================ METHODS ===================
    void swap(MyString& rhs);    
    size_t lenght() const;
    void print() const;
    const int cast_to_int() const;
    const int cast_to_float() const;
    const int cast_to_double() const;

    //========== ITERATOR METHODS ================
    Iterator begin() noexcept;
    Iterator end() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;
    // ConstIterator cbegin() const noexcept;
    // ConstIterator cend() const noexcept;

};

//============== BINARY OPERATORS =================
const MyString operator+(const MyString& lhs, const MyString& rhs);
bool operator==(const MyString& lhs, const MyString& rhs);

std::ostream& operator<<(std::ostream& out, const MyString& string);
