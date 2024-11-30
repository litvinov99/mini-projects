// #include <algorithm>
#include <cstring>

#include "my_string.h"

MyString::MyString() 
        : str(new char[1]) {
    str[0] = '\0';
}

MyString::MyString(const char* other) {
    size_t lenght = strlen(other);
    str = new char[lenght + 1];
    std::memcpy(str, other, lenght);
    str[lenght] = '\0';    
}

MyString::MyString(size_t count, char ch) 
        : str(new char[count]) {
    // std::fill(str[0], str[count - 1], ch);
    std::memset(str, ch, count);
    str[count] = '\0';
}

MyString::MyString(const MyString& other) {

}

MyString::~MyString() {
    delete[] str;
}

MyString& MyString::operator=(const MyString& other) {
    if (this != &other) {
        delete[] str;
        size_t lenght = strlen(other.str);
    }
    return (*this);
}

MyString& MyString::operator+=(const MyString& rhs) {
    MyString result;
    size_t this_lenght = strlen(str);
    size_t rhs_lenght = strlen(rhs.str);
    result.str = new char[this_lenght + rhs_lenght + 1];
    std::memcpy(result.str, str, this_lenght);
    std::memcpy(&result.str[this_lenght], rhs.str, rhs_lenght);
    result.str[this_lenght + rhs_lenght] = '\0';
    this->swap(result);
    return (*this);
}

std::ostream& operator<<(std::ostream& out, const MyString& str) {
    
}

void MyString::swap(MyString& rhs) {
    auto temp = this->str;
    str = rhs.str;
    rhs.str = temp;
}

size_t lenght();

void MyString::print() const {
    size_t lenght = strlen(str);
    for (int i = 0; i < lenght; ++i) {
        std::cout << str[i];
    }
    std::cout << "\n";
}