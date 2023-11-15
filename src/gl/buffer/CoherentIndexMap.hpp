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
                     std::variant<std::monostate, V, std::function<V(std::size_t)>>&
                         clean_up = std::monostate{},
                     bool is_client_storage = false);
    CoherentIndexMap(const CoherentIndexMap& other) = delete;
    CoherentIndexMap(CoherentIndexMap&& other) = default;
    virtual ~CoherentIndexMap();

    V& operator[](std::size_t index);
    const V& operator[](std::size_t index) const;

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

    std::variant<std::monostate, V, std::function<V(std::size_t)>> clean_up;
    std::unordered_set<std::size_t> unused;
};

template <IsTriviallyCopyable V>
CoherentIndexMap<
    V>::CoherentIndexMap(std::size_t capacity,

                         std::variant<std::monostate, V, std::function<V(std::size_t)>>&
                             clean_up,
                         bool is_client_storage)
    : CoherentStorage<V>(capacity, clean_up, is_client_storage)
    , clean_up(clean_up) {
    for (std::size_t i = 0; i < capacity; ++i) {
        unused.insert(unused.end(), i);
    }
}

template <IsTriviallyCopyable V>
CoherentIndexMap<V>::~CoherentIndexMap() {}

template <IsTriviallyCopyable V>
V& CoherentIndexMap<V>::operator[](std::size_t index) {
    if (unused.contains(index)) {
        throw std::out_of_range("");
    }
    return CoherentStorage<V>::operator[](index);
}

template <IsTriviallyCopyable V>
const V& CoherentIndexMap<V>::operator[](std::size_t index) const {
    if (unused.contains(index)) {
        throw std::out_of_range("");
    }
    return CoherentStorage<V>::operator[](index);
}

template <IsTriviallyCopyable V>
std::optional<std::size_t> CoherentIndexMap<V>::initUnusedIndex() {
    auto iter = unused.begin();
    if (iter == unused.end()) {
        return std::nullopt;
    }
    std::size_t index = *iter;
    unused.erase(iter);
    return index;
}

template <IsTriviallyCopyable V>
void CoherentIndexMap<V>::freeIndex(std::size_t index) {
    unused.insert(unused.end(), index);

    if (clean_up.index() == 0) {
        // monostate
    } else if (clean_up.index() == 1) {
        CoherentStorage<V>::operator[](index) = std::get<V>(clean_up);
    } else {
        CoherentStorage<V>::operator[](index) =
            std::get<std::function<V(std::size_t)>>(clean_up)(index);
    }
}

template <IsTriviallyCopyable V>
bool CoherentIndexMap<V>::containsIndex(const std::size_t& index) const {
    return !unused.contains(index);
};

template <IsTriviallyCopyable V>
V CoherentIndexMap<V>::extractIndex(const std::size_t& index) {
    auto value = CoherentStorage<V>::operator[](index);
    freeIndex(index);
    return value;
}

template <IsTriviallyCopyable V>
bool CoherentIndexMap<V>::isEmpty() const {
    return unused.size() == getCapacity();
}

template <IsTriviallyCopyable V>
std::size_t CoherentIndexMap<V>::getSize() const {
    return getCapacity() - unused.size();
}

} // namespace gnev::gl::buffer