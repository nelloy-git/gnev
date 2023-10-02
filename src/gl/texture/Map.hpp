#pragma once

#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "gl/texture/ResizableStorage.hpp"

namespace gnev::gl::texture {

template <typename K>
class EXPORT Map : public ResizableStorage {
public:
    static constexpr GLsizeiptr CAP_MULT = 2;

    Map(const Ctx& ctx,
        std::size_t levels,
        GLenum internalformat,
        std::size_t width,
        std::size_t height,
        std::size_t initial_capacity,
        std::initializer_list<std::pair<K, Image>> initial_data = {});
    Map(const Map& other) = delete;
    Map(Map&& other) = default;
    virtual ~Map();

    void setElement(const K& key, const Image& value);
    std::optional<Image> getElement(const K& key,
                                    std::size_t level,
                                    GLenum format,
                                    GLenum type) const;
    void removeElement(const K& key);

private:
    using ResizableStorage::copyElement;
    using ResizableStorage::getElement;
    using ResizableStorage::setElement;

    using ResizableStorage::copyRange;

    std::unordered_map<K, std::size_t> key_map;
    std::unordered_set<std::size_t> unused_poses;
};

template <typename K>
Map<K>::Map(const Ctx& ctx,
            std::size_t levels,
            GLenum internalformat,
            std::size_t width,
            std::size_t height,
            std::size_t initial_capacity,
            std::initializer_list<std::pair<K, Image>> initial_data)
    : ResizableStorage(ctx,
                       levels,
                       internalformat,
                       width,
                       height,
                       std::max(initial_capacity, initial_data.size())) {
    for (std::size_t i = 0; i < std::max(initial_capacity, initial_data.size()); ++i) {
        unused_poses.insert(i);
    }
    for (auto& init : initial_data) {
        setElement(init.first, init.second);
    }
}

template <typename K>
Map<K>::~Map() {}

template <typename K>
void Map<K>::setElement(const K& key, const Image& value) {
    auto found = key_map.find(key);
    if (found != key_map.end()) {
        ResizableStorage::setElement(found->second, value);
        return;
    }

    if (!unused_poses.empty()) {
        std::size_t pos = *unused_poses.begin();
        unused_poses.erase(unused_poses.begin());
        ResizableStorage::setElement(pos, value);
        key_map[key] = pos;
        return;
    }

    std::size_t prev_cap = capacity;
    ResizableStorage::setCapacity(CAP_MULT * prev_cap);
    for (std::size_t i = prev_cap + 1; i < ResizableStorage<V>::getCapacity(); ++i) {
        unused_poses.insert(i);
    }
    ResizableStorage<V>::setElement(prev_cap, value);
    key_map[key] = prev_cap;
}

template <typename K>
std::optional<Image> Map<K>::getElement(const K& key,
                                        std::size_t level,
                                        GLenum format,
                                        GLenum type) const {
    auto found = key_map.find(key);
    if (found == key_map.end()) {
        return std::nullopt;
    }
    return ResizableStorage::getElement(found->second, level, format, type);
}

template <typename K>
void Map<K>::removeElement(const K& key) {
    auto found = key_map.find(key);
    if (found == key_map.end()) {
        return;
    }

    unused_poses.insert(found->second);
    key_map.erase(found);
}

} // namespace gnev::gl::texture