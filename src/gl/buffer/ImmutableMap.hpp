#pragma once

#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

#include "gl/buffer/ImmutableStorage.hpp"

namespace gnev::gl::buffer {

template <typename K, IsTriviallyCopyable V>
class EXPORT ImmutableMap : public ImmutableStorage<V> {
public:
    static constexpr std::size_t CAP_MULT = 2;

    ImmutableMap(const Ctx& ctx,
                 GLenum storage_flags,
                 std::size_t capacity,
                 std::initializer_list<std::pair<K, V>> initial_data = {});
    ImmutableMap(const ImmutableMap& other) = delete;
    ImmutableMap(ImmutableMap&& other) = default;
    virtual ~ImmutableMap();

    void setElement(const K& key, const V& value);
    std::optional<V> getElement(const K& key) const;
    void removeElement(const K& key);

    std::size_t getSize() const;

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
                                 std::size_t capacity,
                                 std::initializer_list<std::pair<K, V>> initial_data)
    : ImmutableStorage<V>(ctx, usage, std::max(capacity, initial_data.size()), {}) {
    for (std::size_t i = 0; i < ImmutableStorage<V>::getCapacity(); ++i) {
        unused_poses.insert(i);
    }
    for (auto& key_value : initial_data) {
        setElement(key_value.first, key_value.second);
    }
}

template <typename K, IsTriviallyCopyable V>
ImmutableMap<K, V>::~ImmutableMap() {}

template <typename K, IsTriviallyCopyable V>
void ImmutableMap<K, V>::setElement(const K& key, const V& value) {
    auto found = key_map.find(key);
    // Found key
    if (found != key_map.end()) {
        ImmutableStorage<V>::setElement(found->second, value);
        return;
    }

    if (unused_poses.empty()) {
        throw std::out_of_range("");
    }

    std::size_t pos = unused_poses.extract(unused_poses.begin()).value();
    ImmutableStorage<V>::setElement(pos, value);
    key_map[key] = pos;
}

template <typename K, IsTriviallyCopyable V>
std::optional<V> ImmutableMap<K, V>::getElement(const K& key) const {
    auto found = key_map.find(key);
    if (!key_map.contains(found)) {
        return std::nullopt;
    }
    return ImmutableStorage<V>::getElement(found->second);
}

template <typename K, IsTriviallyCopyable V>
void ImmutableMap<K, V>::removeElement(const K& key) {
    auto node = key_map.extract(key);
    if (!node.has_value()) {
        return;
    }
    unused_poses.insert(node.value());
}

template <typename K, IsTriviallyCopyable V>
std::size_t ImmutableMap<K, V>::getSize() const {
    return key_map.size();
}

} // namespace gnev::gl::buffer