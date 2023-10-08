#pragma once

#include <optional>

#include "gl/buffer/ClientStorage.hpp"
#include "util/Util.hpp"

namespace gnev::gl::buffer {

template <typename K, IsTriviallyCopyable V>
class ClientMap {
    static constexpr std::size_t CAP_MULT = 2;

    ClientMap(const Ctx& ctx,
              std::size_t capacity,
              std::initializer_list<std::pair<K, V>> initial_data = {});
    ClientMap(const ClientMap& other) = delete;
    ClientMap(ClientMap&& other) = default;
    virtual ~ClientMap();

    void setElement(const K& key, const V& value);
    std::optional<V> getElement(const K& key) const;
    void removeElement(const K& key);

    std::size_t getSize() const;

private:
    std::unordered_map<K, std::size_t> key_map;
    std::unordered_set<std::size_t> unused_poses;
};

template <typename K, IsTriviallyCopyable V>
ClientMap<K, V>::ClientMap(const Ctx& ctx,
                                 std::size_t capacity,
                                 std::initializer_list<std::pair<K, V>> initial_data)
    : ClientStorage<V>(ctx, std::max(capacity, initial_data.size()), {}) {
    for (std::size_t i = 0; i < ClientStorage<V>::getCapacity(); ++i) {
        unused_poses.insert(i);
    }
    for (auto& key_value : initial_data) {
        setElement(key_value.first, key_value.second);
    }
}

template <typename K, IsTriviallyCopyable V>
ClientMap<K, V>::~ClientMap() {}

template <typename K, IsTriviallyCopyable V>
void ClientMap<K, V>::setElement(const K& key, const V& value) {
    auto found = key_map.find(key);
    // Found key
    if (found != key_map.end()) {
        ClientStorage<V>::setElement(found->second, value);
        return;
    }

    if (unused_poses.empty()) {
        throw std::out_of_range("");
    }

    std::size_t pos = unused_poses.extract(unused_poses.begin()).value();
    ClientStorage<V>::setElement(pos, value);
    key_map[key] = pos;
}

template <typename K, IsTriviallyCopyable V>
std::optional<V> ClientMap<K, V>::getElement(const K& key) const {
    auto found = key_map.find(key);
    if (!key_map.contains(found)) {
        return std::nullopt;
    }
    return ClientStorage<V>::getElement(found->second);
}

template <typename K, IsTriviallyCopyable V>
void ClientMap<K, V>::removeElement(const K& key) {
    auto node = key_map.extract(key);
    if (!node.has_value()) {
        return;
    }
    unused_poses.insert(node.value());
}

template <typename K, IsTriviallyCopyable V>
std::size_t ClientMap<K, V>::getSize() const {
    return key_map.size();
}

} // namespace gnev::gl::buffer