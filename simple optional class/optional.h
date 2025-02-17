#include <stdexcept>
#include <utility>

// Исключение этого типа должно генерироватся при обращении к пустому optional
class BadOptionalAccess : public std::exception {
public:
    using exception::exception;

    virtual const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class Optional {
public:
    Optional() = default;
    Optional(const T& value);
    Optional(T&& value);
    Optional(const Optional& other);
    Optional(Optional&& other);

    Optional& operator=(const T& value);
    Optional& operator=(T&& rhs);
    Optional& operator=(const Optional& rhs);
    Optional& operator=(Optional&& rhs);

    bool operator!=(const Optional& rhs) const {
        if (rhs.HasValue() == this->HasValue()) {
            return (buff_ == rhs.buff_);
        }
        return false;
    }

    bool operator==(const Optional& rhs) const {
        return *this != rhs;
    }

    ~Optional();

    bool HasValue() const;

    T& operator*();
    const T& operator*() const;
    T* operator->();
    const T* operator->() const;

    T& Value();
    const T& Value() const;

    void Reset();

private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)];
    T* buff_ = nullptr;
    bool is_initialized_ = false;
};

// ctor & dtor
template <typename T>
Optional<T>::Optional(const T& value) 
    : buff_(new (&data_[0]) T(value))
    , is_initialized_(true) {
}

template <typename T>
Optional<T>::~Optional() {
    if (is_initialized_) {
        buff_->~T();
    }
}

template <typename T>
Optional<T>::Optional(T&& value) 
    : buff_(new (&data_[0]) T(std::forward<T>(value)))
    , is_initialized_(true) {
}

template <typename T>
Optional<T>::Optional(const Optional& other) {
    if (other.HasValue() && this->HasValue()) {
        *buff_ = other.Value();
    } else if (other.HasValue() && !this->HasValue()) {
        buff_ = new (&data_[0]) T(other.Value());
        is_initialized_ = other.HasValue();
    } else if (!other.HasValue() && this->HasValue()) {
        this->Reset();
    } 
}
template <typename T>
Optional<T>::Optional(Optional&& other) {
    if (other.HasValue() && this->HasValue()) {
        *buff_ = std::forward<T>(other.Value());
    } else if (other.HasValue() && !this->HasValue()) {
        buff_ = new (&data_[0]) T(std::forward<T>(other.Value()));
        is_initialized_ = other.HasValue();
    } else if (!other.HasValue() && this->HasValue()) {
        this->Reset();
    } 
}

template <typename T>
Optional<T>& Optional<T>::operator=(const T& value) {
    if (this->HasValue()) {
        *buff_ = value;
    } else {
        buff_ = new (&data_[0]) T(value);
        is_initialized_ = true;
    }
    return *this;
}

template <typename T>
Optional<T>& Optional<T>::operator=(T&& rhs) {
    if (this->HasValue()) {
        *buff_ = std::forward<T>(rhs);
    } else {
        buff_ = new (&data_[0]) T(std::forward<T>(rhs));
        is_initialized_ = true;
    }
    return *this;
}

template <typename T>
Optional<T>& Optional<T>::operator=(const Optional& rhs) {
    if (this != &rhs) {
        if (rhs.HasValue() && this->HasValue()) {
            *buff_ = rhs.Value();
        } else if (rhs.HasValue() && !this->HasValue()) {
            buff_ = new (&data_[0]) T(rhs.Value());
            is_initialized_ = rhs.HasValue();
        } else if (!rhs.HasValue() && this->HasValue()) {
            this->Reset();
        } 
    }
    return *this;
}

template <typename T>
Optional<T>& Optional<T>::operator=(Optional&& rhs) {
    if (this != &rhs) {
        if (rhs.HasValue() && this->HasValue()) {
            *buff_ = std::forward<T>(rhs.Value());
        } else if (rhs.HasValue() && !this->HasValue()) {
            buff_ = new (&data_[0]) T(std::forward<T>(rhs.Value()));
            is_initialized_ = rhs.HasValue();
        } else if (!rhs.HasValue() && this->HasValue()) {
            this->Reset();
        } 
    }
    return *this;
}

// methods
template <typename T>
bool Optional<T>::HasValue() const {
    return is_initialized_;
}

template <typename T>
T& Optional<T>::operator*() {
    return *buff_;
}

template <typename T>
const T& Optional<T>::operator*() const {
    return *buff_;
}

template <typename T>
T* Optional<T>::operator->() {
    return buff_;
}

template <typename T>
const T* Optional<T>::operator->() const {
    return buff_;
}

template <typename T>
T& Optional<T>::Value() {
    if (!this->HasValue()) {
        throw BadOptionalAccess();
    }
    return *buff_;
}
template <typename T>
const T& Optional<T>::Value() const {
    if (!this->HasValue()) {
        throw BadOptionalAccess();
    }
    return *buff_;
}

template <typename T>
void  Optional<T>::Reset() {
    if (is_initialized_) {
        buff_->~T();
        is_initialized_ = false;
    }
}