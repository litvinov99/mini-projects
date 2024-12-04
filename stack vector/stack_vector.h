#pragma once

#include <array>
#include <stdexcept>

template <typename T, size_t N>
class StackVector {
private:
    std::array<T, N> arr_; 
    size_t capacity_ = N;
    size_t size_;
public:
    explicit StackVector(size_t a_size = 0);

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    typename std::array<T, N>::iterator begin() noexcept ;
    typename std::array<T, N>::iterator end() noexcept ;
    typename std::array<T, N>::const_iterator begin() const noexcept;
    typename std::array<T, N>::const_iterator end() const noexcept;

    size_t Size() const;
    size_t Capacity() const;
    void PushBack(const T& value);
    T PopBack();
};


template <typename T, size_t N>
StackVector<T, N>::StackVector(size_t a_size) 
    : size_(a_size) {
    if (size_ > capacity_)
        throw std::invalid_argument("invalid_argument");
};

template <typename T, size_t N>
T& StackVector<T, N>::operator[](size_t index) {
    if (index < 0 || index > size_)
        throw std::invalid_argument("invalid_index");    
    return (arr_[index]);
}

template <typename T, size_t N>
const T& StackVector<T, N>::operator[](size_t index) const {
    if (index < 0 || index > size_)
        throw std::invalid_argument("invalid_index");    
    return (arr_[index]);
}

template <typename T, size_t N>
typename std::array<T, N>::iterator StackVector<T, N>::begin() noexcept {
    return (arr_.begin());
}

template <typename T, size_t N>
typename std::array<T, N>::iterator StackVector<T, N>::end() noexcept {
    return (arr_.end());
}

template <typename T, size_t N>
typename std::array<T, N>::const_iterator StackVector<T, N>::begin() const noexcept {
    return arr_.begin();
}

template <typename T, size_t N>
typename std::array<T, N>::const_iterator StackVector<T, N>::end() const noexcept {
    return arr_.end();
}

template <typename T, size_t N>
size_t StackVector<T, N>::Size() const {
    return (size_);
}

template <typename T, size_t N>
size_t StackVector<T, N>::Capacity() const {
    return (capacity_);
}

template <typename T, size_t N>
void StackVector<T, N>::PushBack(const T& value) {
    if (size_ == capacity_)
        throw std::overflow_error("overflow_error");
    arr_[size_ + 1] = value;
    ++size_;
}

template <typename T, size_t N>
T StackVector<T, N>::PopBack() {
    if (size_ == 0)
        throw std::underflow_error("underflow_error");
    auto val = arr_[size_];
    --size_;
    return (val);    
}
