#pragma once

#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "gl/buffer/ResizableStorage.hpp"

namespace gnev::gl::buffer {

template <typename K, IsTriviallyCopyable V>
class EXPORT ResizableMap : public ResizableStorage<V> {
public:
    static constexpr GLsizeiptr CAP_MULT = 2;

    ResizableMap(const Ctx& ctx,
                 GLenum usage,
                 std::size_t initial_capacity,
                 std::initializer_list<std::pair<K, V>> initial_data = {});
    ResizableMap(const ResizableMap& other) = delete;
    ResizableMap(ResizableMap&& other) = default;
    virtual ~ResizableMap();

    void setElement(const K& key, const V& value);
    std::optional<V> getElement(const K& key) const;
    void removeElement(const K& key);

private:
    using ResizableStorage<V>::setElement;
    using ResizableStorage<V>::copyElement;
    using ResizableStorage<V>::getElement;

    using ResizableStorage<V>::setRange;
    using ResizableStorage<V>::copyRange;
    using ResizableStorage<V>::getRange;

    std::unordered_map<K, std::size_t> key_map;
    std::unordered_set<std::size_t> unused_poses;
};

template <typename K, IsTriviallyCopyable V>
ResizableMap<K, V>::ResizableMap(const Ctx& ctx,
                                 GLenum usage,
                                 std::size_t initial_capacity,
                                 std::initializer_list<std::pair<K, V>> initial_data)
    : ResizableStorage<V>(ctx,
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
ResizableMap<K, V>::~ResizableMap() {}

template <typename K, IsTriviallyCopyable V>
void ResizableMap<K, V>::setElement(const K& key, const V& value) {
    auto found = key_map.find(key);
    if (found != key_map.end()) {
        ResizableStorage<V>::setElement(found->second, value);
        return;
    }

    if (!unused_poses.empty()) {
        std::size_t pos = *unused_poses.begin();
        unused_poses.erase(unused_poses.begin());
        ResizableStorage<V>::setElement(pos, value);
        key_map[key] = pos;
        return;
    }

    std::size_t previous_capacity = ResizableStorage<V>::getCapacity();
    ResizableStorage<V>::setCapacity(CAP_MULT * previous_capacity);
    for (std::size_t i = previous_capacity + 1; i < ResizableStorage<V>::getCapacity();
         ++i) {
        unused_poses.insert(i);
    }
    ResizableStorage<V>::setElement(previous_capacity, value);
    key_map[key] = previous_capacity;
}

template <typename K, IsTriviallyCopyable V>
std::optional<V> ResizableMap<K, V>::getElement(const K& key) const {
    auto found = key_map.find(key);
    if (found == key_map.end()) {
        return std::nullopt;
    }
    return ResizableStorage<V>::getElement(found->second);
}

template <typename K, IsTriviallyCopyable V>
void ResizableMap<K, V>::removeElement(const K& key) {
    auto found = key_map.find(key);
    if (found == key_map.end()) {
        return;
    }

    unused_poses.insert(found->second);
    key_map.erase(found);
}

} // namespace gnev::gl::buffer