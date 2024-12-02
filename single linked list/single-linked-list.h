#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <initializer_list>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend SingleLinkedList;

        explicit BasicIterator(Node* node) 
            : node_(node) {
        }

    public:
        //==================== DECLARATIONS =========================
        using iterator_category = std::forward_iterator_tag;
        // type of elements that the iterator moves through
        using value_type = Type;
        // type used to store the offset between iterators
        using pointer = ValueType*;
        using difference_type = std::ptrdiff_t;
        using reference = ValueType&;

        //===================== CTOR & DTOR =========================
        BasicIterator() = default;
        BasicIterator(const BasicIterator<Type>& other) noexcept 
            : node_(other.node_) {
        }

        //===================== OPETATORS ===========================
        BasicIterator& operator= (const BasicIterator& rhs) = default;
        
        bool operator== (const BasicIterator<const Type>& rhs) const noexcept {
            return (this->node_ == rhs.node_);
        }

        bool operator!= (const BasicIterator<const Type>& rhs) const noexcept {
            return (this->node_ != rhs.node_);
        }

        BasicIterator operator++() noexcept {
            assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            BasicIterator previous_node(*this);
            ++(*this);
            return previous_node; 
        }

        reference operator*() const noexcept {
            assert(node_ != nullptr);
            return (node_->value);
        }

        pointer operator-> () const noexcept {
            assert(node_ != nullptr);
            return &(node_->value);
        }

    private:
        Node* node_ = nullptr;
    };

public:
    //===================== CTOR & DTOR =========================
    SingleLinkedList() {
    };

    SingleLinkedList(std::initializer_list<Type> values) {
        FillLinkedList(values); 
    }

    SingleLinkedList(const SingleLinkedList<Type>& other) {
        assert(size_ == 0 && head_.next_node == nullptr);
        FillLinkedList(other);  
    }

    template <typename InType>
    void FillLinkedList(const InType& input) {
        SingleLinkedList<Type> temp_reverse_copy;
        for (auto val : input) {
            temp_reverse_copy.PushFront(val);
        }
        for (auto val : temp_reverse_copy) {
            this->PushFront(val);
        } 
    }
    
    ~SingleLinkedList() {
        Clear();
    }

    //====================== DECLARATIONS ========================
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    //=================== ASSIGNMENT OPETATOR ====================
    SingleLinkedList& operator= (const SingleLinkedList<Type>& rhs) {
        if (this != &rhs) {
            SingleLinkedList<Type> temp_copy(rhs);
            swap(temp_copy);
        }
        return *this;
    }

    //======================== METHODS ===========================

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void PopFront() noexcept {
        assert(!IsEmpty());
        Node* node_for_delete_ptr = head_.next_node;
        Node* new_first_node_ptr = node_for_delete_ptr->next_node;
        delete node_for_delete_ptr;
        head_.next_node = new_first_node_ptr;
        --size_;       
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        Node* next_node_ptr = pos.node_->next_node;
        Node* new_node_ptr = new Node(value, next_node_ptr);
        pos.node_->next_node = new_node_ptr;
        ++size_;
        Iterator iter(pos.node_->next_node);
        return iter;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(!IsEmpty());
        Node* node_to_del_ptr = pos.node_->next_node;
        Node* node_following_del_ptr  = node_to_del_ptr->next_node;
        Node* node_previous_del_ptr  = pos.node_;
        node_previous_del_ptr->next_node = node_following_del_ptr;
        delete node_to_del_ptr;
        --size_;
        Iterator iter(node_previous_del_ptr->next_node);
        return iter;
    }

    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            Node* node_for_delete_ptr = head_.next_node;
            Node* new_first_node_ptr = node_for_delete_ptr->next_node;
            delete node_for_delete_ptr;
            head_.next_node = new_first_node_ptr;
        }
        size_ = 0;
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return (size_ == 0);
    }

    void swap(SingleLinkedList<Type>& other) noexcept {
        std::swap(other.head_.next_node, head_.next_node);
        std::swap(other.size_, size_);
    }

    //================== ITERATOR METHODS ========================

    [[nodiscard]] Iterator begin() noexcept {
        Iterator iter(head_.next_node);
        return iter;
    }

    [[nodiscard]] Iterator end() noexcept {
        Iterator iter(nullptr);
        return iter;
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        ConstIterator c_iter(head_.next_node);
        return c_iter;
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        ConstIterator c_iter(nullptr);
        return c_iter;
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return begin();
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return end();
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        Iterator iter(&head_);
        return iter;
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        ConstIterator iter(const_cast<Node*>(&head_));
        return iter;
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
    }


private:
    Node head_;
    size_t size_ = 0;
};

//================ OPERATORS for SingleLinkedList ================

// c нижними операторами что-то не так

template <typename Type>
bool operator== (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), lhs.end());
}

template <typename Type>
bool operator!= (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator< (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.end()));
}

template <typename Type>
bool operator<= (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs < rhs || lhs == rhs);
}

template <typename Type>
bool operator> (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (rhs < lhs);
}

template <typename Type>
bool operator>= (const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);   
}