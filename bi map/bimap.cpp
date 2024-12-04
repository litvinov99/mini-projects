#include "bimap.h"

#include <exception>
#include <deque>
#include <string>
#include <unordered_map>


struct BiMap::Impl {
    
    struct WordsPair {
        std::string key_;
        std::string value_;
    };
    
    std::deque<WordsPair> all_pairs_;
    std::unordered_map<std::string_view, std::string_view> key_to_value_;
    std::unordered_map<std::string_view, std::string_view> value_to_key_;
    
};

BiMap::BiMap() : impl_(std::make_unique<Impl>()) {   
}

BiMap::BiMap(const BiMap& other) : impl_(std::make_unique<Impl>(*other.impl_)) {  
}

BiMap& BiMap::operator=(const BiMap& other) {
    if (this != &other) {
        *impl_ = *other.impl_;
    }
    return (*this);
}

BiMap::BiMap(BiMap&& other) = default;
BiMap& BiMap::operator=(BiMap&& other) = default;
BiMap::~BiMap() = default;

bool BiMap::Add(std::string_view key, std::string_view value) {
    if (impl_->key_to_value_.count(key) == 0 && impl_->value_to_key_.count(value) == 0) {
        std::string str_key{key}; 
        std::string str_value{value};
        Impl::WordsPair new_pair{std::move(str_key), std::move(str_value)};
        impl_->all_pairs_.push_back(new_pair);
        Impl::WordsPair* last_pair = &impl_->all_pairs_.back();
        impl_->key_to_value_.insert({last_pair->key_, last_pair->value_});
        impl_->value_to_key_.insert({last_pair->value_, last_pair->key_});
        return (true);
    }
    return (false);
}

std::optional<std::string_view> BiMap::FindValue(std::string_view key) const noexcept {
    auto iter = impl_->key_to_value_.find(key);
    if (iter != impl_->key_to_value_.end()) {
        return (iter->second);
    }
    return (std::nullopt);
}

std::optional<std::string_view> BiMap::FindKey(std::string_view value) const noexcept {
    auto iter = impl_->value_to_key_.find(value);
    if (iter != impl_->key_to_value_.end()) {
        return (iter->second);
    }
    return (std::nullopt);
}