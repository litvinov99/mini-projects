#include <cstring>
#include <utility>

#include "my_string.h"

MyString::MyString(const char* other) 
        : buffer_(new char[strlen(other) + 1])
        , len_(strlen(other)) {
    std::memcpy(buffer_, other, strlen(other));
    buffer_[strlen(other)] = '\0';    
}

MyString::MyString(size_t count, char ch) 
        : buffer_(new char[count + 1])
        , len_(count) {
    std::memset(buffer_, ch, count);
    buffer_[count] = '\0';
}

MyString::MyString(const MyString& other) {
    if (other.buffer_ != nullptr) {
        len_ = strlen(other.buffer_);
        buffer_ = new char[len_];
        std::memcpy(buffer_, other.buffer_, len_);    
    } else {
       buffer_ = nullptr; 
    }
}

MyString::MyString(MyString&& other) 
        : buffer_(std::move(other.buffer_))
        , len_(std::exchange(other.len_, 0)) {
}

MyString::~MyString() {
    delete[] buffer_;
}

MyString& MyString::operator=(const MyString& other) noexcept {
    if (this != &other) {
        MyString temp_copy(other);
        this->swap(temp_copy);
    }
    return (*this);
}

MyString& MyString::operator=(MyString&& other) noexcept {
    if (this != &other) {
        MyString temp_copy(other);
        this->swap(temp_copy);
    }
    return (*this);
}

char& MyString::operator[](size_t index) noexcept {
    return (buffer_[index]);
}
const char& MyString::operator[](size_t index) const noexcept {
    return (buffer_[index]);
}

MyString& MyString::operator+=(const MyString& rhs) {
    MyString result;
    result.buffer_ = new char[len_ + rhs.len_ + 1];
    std::memcpy(result.buffer_, buffer_, len_);
    std::memcpy(&result.buffer_[rhs.len_], rhs.buffer_, rhs.len_);
    result.buffer_[len_ + rhs.len_] = '\0';
    this->swap(result);
    return (*this);
}

bool MyString::operator!=(const MyString& rhs) const {
    if (len_ == rhs.len_) {
        return (std::lexicographical_compare(this->begin(), this->end(), 
            rhs.begin(), rhs.end()));
    }
    return (false);
}

void MyString::swap(MyString& rhs) {
    std::swap(buffer_, rhs.buffer_);
    std::swap(len_, rhs.len_);
}

size_t MyString::lenght() const {
    return (len_);
}

void MyString::print() const {
    size_t lenght = strlen(buffer_);
    for (size_t i = 0; i < lenght; ++i) {
        std::cout << buffer_[i];
    }
    std::cout << "\n";
}

MyString::Iterator MyString::begin() noexcept {
    return (buffer_);
}

MyString::Iterator MyString::end() noexcept {
    return (std::next(buffer_, len_));
}

MyString::ConstIterator MyString::begin() const noexcept {
    return (buffer_);
}
MyString::ConstIterator MyString::end() const noexcept {
    return (std::next(buffer_, len_));
}

const MyString operator+(const MyString& lhs, const MyString& rhs) {
    MyString result = lhs;
    result += rhs;
    return (result);
}

bool operator==(const MyString& lhs, const MyString& rhs) {
    return !(lhs != rhs);
}


std::ostream& operator<<(std::ostream& out, const MyString& str) {
    for (char ch : str) {
        out << ch;
    }
    return out;     
}