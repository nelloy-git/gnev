#pragma once

#include <optional>

#include "gl/buffer/CoherentStorage.hpp"

namespace gnev::gl::buffer {

template <typename K, IsTriviallyCopyable V>
class CoherentMap : public CoherentStorage<V> {
public:
    CoherentMap(const Ctx& ctx,
                std::size_t capacity,
                std::initializer_list<std::pair<K, V>> initial_data = {},
                const V& initial_value = V{},
                bool is_client_storage = false);
    CoherentMap(const CoherentMap& other) = delete;
    CoherentMap(CoherentMap&& other) = default;
    virtual ~CoherentMap();

    V& operator[](const K& pos);
    const V& operator[](const K& pos) const;

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

    std::unordered_map<K, std::size_t> key_map;
    std::unordered_set<std::size_t> unused_poses;
};

template <typename K, IsTriviallyCopyable V>
CoherentMap<K, V>::CoherentMap(const Ctx& ctx,
                               std::size_t capacity,
                               std::initializer_list<std::pair<K, V>> initial_data,
                               const V& initial_value,
                               bool is_client_storage)
    : CoherentStorage<V>(ctx, capacity, initial_value, is_client_storage) {
    if (capacity < initial_data.size()) {
        throw std::out_of_range("");
    }

    std::size_t i = 0;
    for (const auto& iter : initial_data) {
        key_map[iter.first] = i;
        CoherentStorage<V>::operator[](i) = iter.second;
        ++i;
    }

    for (std::size_t i = initial_data.size(); i < capacity; ++i) {
        unused_poses.insert(i);
        CoherentStorage<V>::operator[](i) = initial_value;
    }
}

template <typename K, IsTriviallyCopyable V>
CoherentMap<K, V>::~CoherentMap() {}

template <typename K, IsTriviallyCopyable V>
V& CoherentMap<K, V>::operator[](const K& key) {
    std::size_t pos;

    auto iter = key_map.find(key);
    if (iter == key_map.end()) {
        pos = unused_poses.extract(unused_poses.begin()).value();
        key_map[key] = pos;
    } else {
        pos = iter->second;
    }
    return CoherentStorage<V>::operator[](pos);
}

template <typename K, IsTriviallyCopyable V>
const V& CoherentMap<K, V>::operator[](const K& key) const {
    auto iter = key_map.find(key);
    if (iter == key_map.end()) {
        throw std::out_of_range("");
    }
    return CoherentStorage<V>::operator[](iter.second);
}

template <typename K, IsTriviallyCopyable V>
bool CoherentMap<K, V>::isEmpty() const {
    return key_map.empty();
}

template <typename K, IsTriviallyCopyable V>
std::size_t CoherentMap<K, V>::getSize() const {
    return key_map.size();
}

} // namespace gnev::gl::buffer