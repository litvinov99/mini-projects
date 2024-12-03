#pragma once

#include "array_ptr.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <initializer_list>
#include <utility>


class ReserveProxyObj {
private:
    size_t capacity_;
public:
    explicit ReserveProxyObj(size_t& capacity_to_reserve)
    :capacity_(capacity_to_reserve) {
    }

    size_t GetCapacity() {
        return capacity_;
    }
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
};

template <typename Type>
class SimpleVector {
private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    ArrayPtr<Type> items_;
    
public:
    //====================== DECLARATIONS ========================

    using Iterator = Type*;
    using ConstIterator = const Type*;

    //===================== CTORS & DTOR =========================

    SimpleVector() noexcept = default;
    explicit SimpleVector(size_t size);
    SimpleVector(ReserveProxyObj obj);
    SimpleVector(size_t size, const Type& value);
    SimpleVector(std::initializer_list<Type> values);
    SimpleVector(const SimpleVector& other);
    SimpleVector(SimpleVector&& other);

    //======================= OPERATORS ==========================

    SimpleVector<Type>& operator= (const SimpleVector& other) noexcept;
    SimpleVector<Type>& operator= (SimpleVector&& other) noexcept;
    Type& operator[](size_t index) noexcept;
    const Type& operator[](size_t index) const noexcept;

    //======================== METHODS ===========================

    void PushBack(const Type& value);
    void PushBack(Type&& value);
    void PopBack() noexcept;
    Iterator Insert(ConstIterator pos, const Type& value);
    Iterator Insert(ConstIterator pos, Type&& value);
    Iterator Erase(ConstIterator pos);

    size_t GetSize() const noexcept;
    size_t GetCapacity() const noexcept;
    bool IsEmpty() const noexcept;

    Type& At(size_t index);
    const Type& At(size_t index) const;

    void Clear() noexcept;
    void Resize(size_t new_size);
    void Reserve(size_t new_capacity);
    void swap(SimpleVector<Type>& rhs) noexcept;

    //================== ITERATOR METHODS ========================

    Iterator begin() noexcept;
    Iterator end() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;

    //=== BINARY OPERATORS are declared and defined at the end ===
};


template <typename Type>
SimpleVector<Type>::SimpleVector(size_t size)
    : SimpleVector(size, Type{}) {
}

template <typename Type>
SimpleVector<Type>::SimpleVector(ReserveProxyObj obj) {
    this->Reserve(obj.GetCapacity());
}

template <typename Type>
SimpleVector<Type>::SimpleVector(size_t size, const Type& value)
    : size_(size)
    , capacity_(size)
    , items_(size) {
        std::fill(this->begin(), this->end(), value);      
}

template <typename Type>
SimpleVector<Type>::SimpleVector(std::initializer_list<Type> values)
    : size_(values.size())
    , capacity_(values.size())
    , items_(values.size()) {
        std::copy(values.begin(), values.end(), this->begin());    
}

template <typename Type>
SimpleVector<Type>::SimpleVector(const SimpleVector& other) {
    ArrayPtr<Type> temp_items(other.GetSize());
    std::copy(other.begin(), other.end(), temp_items.GetRawPtr());
    size_ = other.size_;
    capacity_ = other.capacity_;
    items_.swap(temp_items);
}

template <typename Type>
SimpleVector<Type>::SimpleVector(SimpleVector&& other) 
    : items_(std::move(other.items_)) {
        size_ = std::exchange(other.size_, 0);
        capacity_ = std::exchange(other.capacity_, 0);
}

template <typename Type>
SimpleVector<Type>& SimpleVector<Type>::operator=(const SimpleVector<Type>& other) noexcept {
    if (this != &other) {
        SimpleVector<Type> temp_copy(other);
        this->swap(temp_copy);
    }
    return (*this);
}

template <typename Type>
SimpleVector<Type>& SimpleVector<Type>::operator=(SimpleVector&& other) noexcept {
    if (this != &other) {
        SimpleVector<Type> temp_items(other);
        this->swap(temp_items);
    }
    return (*this);
}

template <typename Type>
Type& SimpleVector<Type>::operator[](size_t index) noexcept {
    assert(index < size_);
    return (items_[index]);
}

template <typename Type>
const Type& SimpleVector<Type>::operator[](size_t index) const noexcept {
    assert(index < size_);
    return (items_[index]);
}

template <typename Type>
void SimpleVector<Type>::PushBack(const Type& value) {
    if (size_ < capacity_) {
        items_[size_] = value;
        ++size_;
    } else {
        size_t new_size = (size_ == 0) ? 1 : capacity_ * 2;
        this->Reserve(new_size);
        items_[size_] = value;
         ++size_;
    }
}

template <typename Type>
void SimpleVector<Type>::PushBack(Type&& value) {
    if (size_ < capacity_) {
        items_[size_] = std::move(value);
        ++size_;
    } else {
        size_t new_size = (size_ == 0) ? 1 : capacity_ * 2;
        this->Reserve(new_size);
        items_[size_] = std::move(value);
        ++size_;
    }
}

template <typename Type>
void SimpleVector<Type>::PopBack() noexcept {
    assert(size_ > 0);
    --size_;
}

template <typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::Insert(ConstIterator pos, const Type& value) {
    assert(pos >= this->begin() && pos <= this->end());
    size_t distance = pos - begin();
    if (size_ < capacity_) {
        std::copy_backward(begin() + distance, end(), end() + 1);
        items_[distance] = std::move(value);
    } else {
        size_t new_size = (size_ == 0) ? 1 : capacity_ * 2;
        this->Reserve(new_size);
        std::copy_backward(begin() + distance, end(), end() + 1);
        items_[distance] = std::move(value);
    }
    ++size_;
    return (begin() + distance);
}

template <typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::Insert(ConstIterator pos, Type&& value) {
    assert(pos >= this->begin() && pos <= this->end());
    size_t distance = pos - begin();
    if (size_ < capacity_) {
        std::move_backward(begin() + distance, end(), end() + 1);
        items_[distance] = std::move(value);
    } else {
        size_t new_size = (size_ == 0) ? 1 : capacity_ * 2;
        this->Reserve(new_size);
        std::move_backward(begin() + distance, end(), end() + 1);
        items_[distance] = std::move(value);
    }
    ++size_;
    return (begin() + distance);
}

template <typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::Erase(ConstIterator pos){
    Iterator iter = const_cast<Iterator>(pos); 
    std::move(std::next(iter), this->end(), iter);
    this->PopBack();
    return (iter);
}

template <typename Type>
size_t SimpleVector<Type>::GetSize() const noexcept {
    return (size_);
}

template <typename Type>
size_t SimpleVector<Type>::GetCapacity() const noexcept {
    return (capacity_);
}

template <typename Type>
bool SimpleVector<Type>::IsEmpty() const noexcept {
    return (size_ == 0);
}

template <typename Type>
Type& SimpleVector<Type>::At(size_t index) {
    if (index >= size_) {
        throw std::out_of_range("You enter invalid out of range index");
    }
    return items_[index];
}

template <typename Type>
const Type& SimpleVector<Type>::At(size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("You enter invalid out of range index");
    }
    return items_[index];
}

template <typename Type>
void SimpleVector<Type>::Clear() noexcept {
    size_ = 0;
}

template <typename Type>
void SimpleVector<Type>::Resize(size_t new_size) {
    if (new_size < size_) {
        size_ = new_size;
        return;
    } else if (new_size > size_ && new_size < capacity_) {
        std::generate(this->end(), this->begin() + new_size, []() { return Type(); });
        size_ = new_size;
    } else if (new_size > capacity_) {
        new_size = std::max(new_size, capacity_ * 2);
        ArrayPtr<Type> temp_items(new_size);
        std::generate(&temp_items[0], &temp_items[new_size], []() { return Type(); });
        std::move(begin(), end(), &temp_items[0]);
        items_.swap(temp_items);
        size_ = new_size;
        capacity_ = new_size;
    }
}

template<typename Type>
void SimpleVector<Type>::Reserve(size_t new_capacity) {
    if (new_capacity > capacity_) {
        ArrayPtr<Type> temp_items(new_capacity);
        if (!this->IsEmpty()) {
            // std::copy(this->begin(), this->end(), temp_items.GetRawPtr());
            std::move(this->begin(), this->end(), temp_items.GetRawPtr());
        }
        items_.swap(temp_items); 
        capacity_ = new_capacity;
    }
}

template <typename Type>
void SimpleVector<Type>::swap(SimpleVector<Type>& rhs) noexcept {
    items_.swap(rhs.items_);
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
}

template <typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::begin() noexcept {
    return (items_.GetRawPtr());
}

template <typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::end() noexcept {
    return (std::next(items_.GetRawPtr(), size_));
}

template <typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::begin() const noexcept {
    return (this->cbegin());
}

template <typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::end() const noexcept {
    return (this->cend());
}

template <typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::cbegin() const noexcept {
    return (items_.GetRawPtr());
}

template <typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::cend() const noexcept {
    return (std::next(items_.GetRawPtr(), size_));
}

//================ BINARY OPERATORS FOR SimpleVector ClASS ================

template <typename Type>
inline bool operator== (const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    if (lhs.GetSize() == rhs.GetSize()) {
        return (std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
    }
    return false;
}

template <typename Type>
inline bool operator!= (const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator< (const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
}

template <typename Type>
inline bool operator<= (const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
inline bool operator> (const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (rhs < lhs);
}

template <typename Type>
inline bool operator>= (const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs > lhs);
}