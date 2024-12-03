#pragma once

#include <cassert>
#include <initializer_list>
#include <stdexcept>

template <typename Type>
class ArrayPtr {
private:
    Type* ptr_ = nullptr;

public:
    //===================== CONST & DEST =========================

    ArrayPtr() = default;

    explicit ArrayPtr(Type* raw_ptr) noexcept {
         ptr_ = raw_ptr;
    }

    explicit ArrayPtr(size_t size) noexcept {
        if  (size > 0) {
            ptr_ = new Type[size];
        }
    }

    explicit ArrayPtr(ArrayPtr&& other) noexcept {
        if  (ptr_ != other.ptr_) {
            ptr_ = std::move(other.ptr_);
            other.ptr_ = nullptr;
        }
    }

    ArrayPtr(const ArrayPtr& other) = delete;

    ~ArrayPtr() {
        delete[] ptr_;    
    }

    //======================= OPERATORS ==========================
    
    ArrayPtr& operator= (const ArrayPtr& other) = delete;

    ArrayPtr& operator=(ArrayPtr&& other) {
        if (ptr_ != other.ptr_) {
            ptr_ = std::move(other.ptr_);
            other.ptr_ = nullptr;
        }
        return *this;
    }

    Type& operator[] (size_t index) noexcept {
        return (ptr_[index]);
    }

    const Type& operator[] (size_t index) const noexcept{
        return (ptr_[index]);
    }

    //======================== METHODS ===========================

    Type* GetRawPtr() const noexcept {
        return ptr_;
    }

    void swap(ArrayPtr& other) noexcept {
        Type* temp_ptr = ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = temp_ptr;
    }

    explicit operator bool() const noexcept {
        if (ptr_ != nullptr) {
            return true;
        }
        return false;
    } 
};