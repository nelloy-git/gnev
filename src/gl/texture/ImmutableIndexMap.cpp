#include "gl/texture/ImmutableIndexMap.hpp"

namespace gnev::gl::texture {

ImmutableIndexMap::ImmutableIndexMap(std::size_t levels,
                                     std::size_t width,
                                     std::size_t height,
                                     std::size_t capacity,
                                     GLenum internal_format,
                                     CleanUp clean_up)
    : ImmutableStorage(levels, width, height, capacity, internal_format)
    , clean_up(clean_up) {
    for (std::size_t i = 0; i < capacity; ++i) {
        unused.insert(unused.end(), i);
    }
}

ImmutableIndexMap::~ImmutableIndexMap() {}

ImmutableStorageIterator ImmutableIndexMap::operator[](std::size_t index) {
    if (unused.contains(index)) {
        throw std::out_of_range("");
    }
    return ImmutableStorage::operator[](index);
}

const ImmutableStorageIterator ImmutableIndexMap::operator[](std::size_t index) const {
    if (unused.contains(index)) {
        throw std::out_of_range("");
    }
    return ImmutableStorage::operator[](index);
}

std::optional<std::size_t> ImmutableIndexMap::initUnusedIndex() {
    auto iter = unused.begin();
    if (iter == unused.end()) {
        return std::nullopt;
    }
    std::size_t index = *iter;
    unused.erase(iter);
    return index;
}

void ImmutableIndexMap::freeIndex(std::size_t index) {
    unused.insert(unused.end(), index);
    if (clean_up.has_value()) {
        clean_up.value()(*this, index);
    }
}

bool ImmutableIndexMap::containsIndex(const std::size_t& index) const {
    return !unused.contains(index);
};

} // namespace gnev::gl::texture