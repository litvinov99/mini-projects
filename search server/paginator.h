#pragma once

#include<vector>

template <typename Iterator>
class IteratorRange {
public:

    IteratorRange(Iterator begin_iter,  Iterator end_iter) 
        : beg_(begin_iter)
        , end_(end_iter) {
    }

    Iterator begin() const {
        return beg_;
    }

    Iterator end() const {
        return end_;
    }

    auto size() const {
        return distance(beg_, end_);
    } 

private:
    Iterator beg_;
    Iterator end_;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const IteratorRange<Iterator>& range) {
    for (auto i = range.begin(); i != range.end(); ++i) {
        out << *i;
    }
    return out;
} 

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator range_begin, Iterator range_end, int page_size) {

        for (int left = distance(range_begin, range_end); left > 0;) {
            const size_t current_page_size = std::min(page_size, left);
            const Iterator current_page_end = next(range_begin, current_page_size);
            pages_.push_back({range_begin, current_page_end});
 
            left -= current_page_size;
            range_begin = current_page_end;
        }
    }

    auto begin() const {
        return pages_.begin();
    }
 
    auto end() const {
        return pages_.end();
    }
 
    size_t size() const {
        return pages_.size();
    }

private:
    std::vector<IteratorRange<Iterator>> pages_;
};


template <typename ContainerType>
auto Paginate(const ContainerType& documents, size_t page_size) {
    return Paginator(begin(documents), end(documents), page_size);
};