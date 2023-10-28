#pragma once

#include <optional>

#include "gl/texture/ImmutableStorage.hpp"

namespace gnev::gl::texture {

class EXPORT ImmutableIndexMap : public ImmutableStorage {
public:
    using CleanUp = std::optional<std::function<void(ImmutableIndexMap&, std::size_t)>>;

    ImmutableIndexMap(std::size_t levels,
                      std::size_t width,
                      std::size_t height,
                      std::size_t capacity,
                      GLenum internal_format,
                      CleanUp clean_up = {});
    ImmutableIndexMap(const ImmutableIndexMap& other) = delete;
    ImmutableIndexMap(ImmutableIndexMap&& other) = default;
    virtual ~ImmutableIndexMap();

    ImmutableStorageIterator operator[](std::size_t index);
    const ImmutableStorageIterator operator[](std::size_t index) const;

    std::optional<std::size_t> initUnusedIndex();
    void freeIndex(std::size_t index);
    bool containsIndex(const std::size_t& index) const;

private:
    CleanUp clean_up;
    std::unordered_set<std::size_t> unused;
};

} // namespace gnev::gl::texture