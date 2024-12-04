#pragma once

#include <cassert>

template <typename T>
class ScopedPtr {
public:
    ScopedPtr() = default;

    explicit ScopedPtr(T* raw_ptr) noexcept {
        ptr_ = raw_ptr;  
    }

    ScopedPtr(const ScopedPtr&) = delete;

    ~ScopedPtr() {
        delete ptr_;
    }

    T* GetRawPtr() const noexcept {
        return ptr_;
    }

    T* Release() noexcept {
        T* copy_ptr = ptr_;
        ptr_ = nullptr;
        return copy_ptr;
    }

    explicit operator bool() const noexcept {
        if (ptr_ != nullptr) {
            return true;
        }
        return false;
    }

    T& operator* () const {
        if (!ptr_) {
            throw std::logic_error("Ошибка");
        }
        return *ptr_;
    }

    T* operator-> () const {
        if (!ptr_) {
            throw std::logic_error("Ошибка");
        }
        return ptr_;
    }

private:
    T* ptr_ = nullptr;
};
