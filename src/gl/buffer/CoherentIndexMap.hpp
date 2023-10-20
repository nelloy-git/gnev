#pragma once

#include <algorithm>
#include <optional>
#include <set>
#include <stdexcept>
#include <unordered_set>

#include "gl/buffer/CoherentStorage.hpp"

namespace gnev::gl::buffer {

template <IsTriviallyCopyable V>
class CoherentIndexMap : public CoherentStorage<V> {
public:
    CoherentIndexMap(std::size_t capacity,
                     const V& initial_value = V{},
                     bool is_client_storage = false);
    CoherentIndexMap(const CoherentIndexMap& other) = delete;
    CoherentIndexMap(CoherentIndexMap&& other) = default;
    virtual ~CoherentIndexMap();

    V& operator[](const std::size_t& index);
    const V& operator[](const std::size_t& index) const;

    std::optional<std::size_t> initUnusedIndex();
    void freeIndex(std::size_t index);
    bool containsIndex(const std::size_t& index) const;
    V extractIndex(const std::size_t& index);

    bool isEmpty() const;
    std::size_t getSize() const;
    using CoherentStorage<V>::getCapacity;

private:
    using CoherentStorage<V>::at;
    using CoherentStorage<V>::operator[];

    using CoherentStorage<V>::back;
    using CoherentStorage<V>::front;

    using CoherentStorage<V>::begin;
    using CoherentStorage<V>::end;

    using CoherentStorage<V>::data;

    V initial_value;
    std::unordered_set<std::size_t> unused;
};

template <IsTriviallyCopyable V>
CoherentIndexMap<V>::CoherentIndexMap(std::size_t capacity,
                                      const V& initial_value,
                                      bool is_client_storage)
    : CoherentStorage<V>(capacity, initial_value, is_client_storage)
    , initial_value(initial_value) {

    std::fill_n(data(), capacity, initial_value);
    for (std::size_t i = 0; i < capacity; ++i) {
        unused.insert(unused.end(), i);
    }
}

template <IsTriviallyCopyable V>
CoherentIndexMap<V>::~CoherentIndexMap() {}

template <IsTriviallyCopyable V>
V& CoherentIndexMap<V>::operator[](const std::size_t& index) {
    if (unused.contains(index)) {
        throw std::out_of_range("");
    }
    return CoherentStorage<V>::operator[](index);
}

template <IsTriviallyCopyable V>
const V& CoherentIndexMap<V>::operator[](const std::size_t& index) const {
    if (unused.contains(index)) {
        throw std::out_of_range("");
    }
    return CoherentStorage<V>::operator[](index);
}

template <IsTriviallyCopyable V>
std::optional<std::size_t> CoherentIndexMap<V>::initUnusedIndex() {
    auto iter = unused.begin();
    if (iter == unused.end()){
        return std::nullopt;
    }
    std::size_t index = *iter;
    unused.erase(iter);
    return index;
}

template <IsTriviallyCopyable V>
void CoherentIndexMap<V>::freeIndex(std::size_t index) {
    unused.insert(unused.end(), index);
    CoherentStorage<V>::operator[](index) = initial_value;
}

template <IsTriviallyCopyable V>
bool CoherentIndexMap<V>::containsIndex(const std::size_t& index) const {
    return !unused.contains(index);
};

template <IsTriviallyCopyable V>
V extractIndex(const std::size_t& index) {
    
}

template <IsTriviallyCopyable V>
bool CoherentIndexMap<V>::contains(const K& key) const {
    return key_map.contains(key);
}

template <IsTriviallyCopyable V>
V CoherentIndexMap<V>::extract(const K& key) {
    auto iter = key_map.find(key);
    if (iter == key_map.end()) {
        throw std::out_of_range("");
    }

    V value = CoherentStorage<V>::operator[](iter->second);
    unused_poses.insert(iter->second);
    key_map.erase(iter);
    return value;
}

template <IsTriviallyCopyable V>
bool CoherentIndexMap<V>::isEmpty() const {
    return key_map.empty();
}

template <IsTriviallyCopyable V>
std::size_t CoherentIndexMap<V>::getSize() const {
    return key_map.size();
}

} // namespace gnev::gl::buffer