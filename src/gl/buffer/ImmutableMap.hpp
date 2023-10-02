#pragma once

#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "gl/buffer/ImmutableStorage.hpp"

namespace gnev::gl::buffer {

template <typename K, IsTriviallyCopyable V>
class EXPORT ImmutableMap : public ImmutableStorage<V> {
public:
    static constexpr std::size_t CAP_MULT = 2;

    ImmutableMap(const Ctx& ctx,
                 GLenum usage,
                 std::size_t initial_capacity,
                 std::initializer_list<std::pair<K, V>> initial_data = {});
    ImmutableMap(const ImmutableMap& other) = delete;
    ImmutableMap(ImmutableMap&& other) = default;
    virtual ~ImmutableMap();

    void setElement(const K& key, const V& value);
    std::optional<V> getElement(const K& key) const;
    void removeElement(const K& key);

private:
    using ImmutableStorage<V>::setElement;
    using ImmutableStorage<V>::copyElement;
    using ImmutableStorage<V>::getElement;

    using ImmutableStorage<V>::setRange;
    using ImmutableStorage<V>::copyRange;
    using ImmutableStorage<V>::getRange;

    std::unordered_map<K, std::size_t> key_map;
    std::unordered_set<std::size_t> unused_poses;
};

template <typename K, IsTriviallyCopyable V>
ImmutableMap<K, V>::ImmutableMap(const Ctx& ctx,
                                 GLenum usage,
                                 std::size_t initial_capacity,
                                 std::initializer_list<std::pair<K, V>> initial_data)
    : ImmutableStorage<V>(ctx,
                          usage,
                          std::max(initial_capacity, initial_data.size()),
                          {}) {
    for (std::size_t i = 0; i < std::max(initial_capacity, initial_data.size()); ++i) {
        unused_poses.insert(i);
    }
    for (auto& init : initial_data) {
        setElement(init.first, init.second);
    }
}

template <typename K, IsTriviallyCopyable V>
ImmutableMap<K, V>::~ImmutableMap() {}

template <typename K, IsTriviallyCopyable V>
void ImmutableMap<K, V>::setElement(const K& key, const V& value) {
    auto found = key_map.find(key);
    if (found != key_map.end()) {
        ImmutableStorage<V>::setElement(found->second, value);
        return;
    }

    if (!unused_poses.empty()) {
        std::size_t pos = *unused_poses.begin();
        unused_poses.erase(unused_poses.begin());
        ImmutableStorage<V>::setElement(pos, value);
        key_map[key] = pos;
        return;
    }

    std::size_t previous_capacity = ImmutableStorage<V>::getCapacity();
    ImmutableStorage<V>::setCapacity(CAP_MULT * previous_capacity);
    for (std::size_t i = previous_capacity + 1; i < ImmutableStorage<V>::getCapacity();
         ++i) {
        unused_poses.insert(i);
    }
    ImmutableStorage<V>::setElement(previous_capacity, value);
    key_map[key] = previous_capacity;
}

template <typename K, IsTriviallyCopyable V>
std::optional<V> ImmutableMap<K, V>::getElement(const K& key) const {
    auto found = key_map.find(key);
    if (found == key_map.end()) {
        return std::nullopt;
    }
    return ImmutableStorage<V>::getElement(found->second);
}

template <typename K, IsTriviallyCopyable V>
void ImmutableMap<K, V>::removeElement(const K& key) {
    auto found = key_map.find(key);
    if (found == key_map.end()) {
        return;
    }

    unused_poses.insert(found->second);
    key_map.erase(found);
}

} // namespace gnev::gl::buffer