#pragma once

#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "gl/buffer/Resizable.hpp"

namespace gnev::gl::buffer {

template <typename K, IsTriviallyCopyable V>
class EXPORT Map : public Resizable<V> {
public:
    Map(const Ctx& ctx,
        GLenum usage,
        std::size_t initial_capacity,
        std::initializer_list<std::pair<K, V>> initial_data = {});
    Map(const Map& other) = delete;
    Map(Map&& other) = default;
    virtual ~Map();

    void setElement(const K& key, const V& value);
    std::optional<V> getElement(const K& key) const;
    void removeElement(const K& key);

private:
    using Resizable<V>::setElement;
    using Resizable<V>::copyElement;
    using Resizable<V>::getElement;

    using Resizable<V>::setRange;
    using Resizable<V>::copyRange;
    using Resizable<V>::getRange;

    std::unordered_map<K, std::size_t> key_map;
    std::unordered_set<std::size_t> unused_poses;
};

template <typename K, IsTriviallyCopyable V>
Map<K, V>::Map(const Ctx& ctx,
               GLenum usage,
               std::size_t initial_capacity,
               std::initializer_list<std::pair<K, V>> initial_data)
    : Resizable<V>(ctx, usage, initial_capacity, {}) {
    for (std::size_t i = 0; i < initial_capacity; ++i) {
        unused_poses.insert(i);
    }
    for (auto& init : initial_data) {
        setElement(init.first, init.second);
    }
}

template <typename K, IsTriviallyCopyable V>
Map<K, V>::~Map() {}

template <typename K, IsTriviallyCopyable V>
void Map<K, V>::setElement(const K& key, const V& value) {
    auto found = key_map.find(key);
    if (found != key_map.end()) {
        Resizable<V>::setElement(found->second, value);
        return;
    }

    if (!unused_poses.empty()) {
        std::size_t pos = *unused_poses.begin();
        unused_poses.erase(unused_poses.begin());
        Resizable<V>::setElement(pos, value);
        key_map[key] = pos;
        return;
    }

    std::size_t previous_capacity = Resizable<V>::getCapacity();
    Resizable<V>::setCapacity(2 * previous_capacity);
    for (std::size_t i = previous_capacity + 1; i < Resizable<V>::getCapacity(); ++i) {
        unused_poses.insert(i);
    }
    Resizable<V>::setElement(previous_capacity, value);
    key_map[key] = previous_capacity;
}

template <typename K, IsTriviallyCopyable V>
std::optional<V> Map<K, V>::getElement(const K& key) const {
    auto found = key_map.find(key);
    if (found == key_map.end()) {
        return std::nullopt;
    }
    return Resizable<V>::getElement(found->second);
}

template <typename K, IsTriviallyCopyable V>
void Map<K, V>::removeElement(const K& key) {
    auto found = key_map.find(key);
    if (found == key_map.end()) {
        return;
    }

    unused_poses.insert(found->second);
    key_map.erase(found);
}

} // namespace gnev::gl::buffer