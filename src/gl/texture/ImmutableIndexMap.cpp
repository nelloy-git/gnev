#include "gl/texture/ImmutableIndexMap.hpp"

namespace gnev::gl::texture {

ImmutableIndexMap::ImmutableIndexMap(GLuint levels,
                                     GLuint width,
                                     GLuint height,
                                     GLuint capacity,
                                     GLenum internal_format,
                                     CleanUp clean_up)
    : ImmutableStorage(levels, width, height, capacity, internal_format)
    , clean_up(clean_up) {
    for (GLuint i = 0; i < capacity; ++i) {
        unused.insert(unused.end(), i);
    }
}

ImmutableIndexMap::~ImmutableIndexMap() {}

ImmutableStorageIterator ImmutableIndexMap::operator[](GLuint index) {
    if (unused.contains(index)) {
        throw std::out_of_range("");
    }
    return ImmutableStorage::operator[](index);
}

const ImmutableStorageIterator ImmutableIndexMap::operator[](GLuint index) const {
    if (unused.contains(index)) {
        throw std::out_of_range("");
    }
    return ImmutableStorage::operator[](index);
}

std::optional<GLuint> ImmutableIndexMap::initUnusedIndex() {
    auto iter = unused.begin();
    if (iter == unused.end()) {
        return std::nullopt;
    }
    GLuint index = *iter;
    unused.erase(iter);
    return index;
}

void ImmutableIndexMap::freeIndex(GLuint index) {
    unused.insert(unused.end(), index);
    if (clean_up.has_value()) {
        clean_up.value()(*this, index);
    }
}

bool ImmutableIndexMap::containsIndex(const GLuint& index) const {
    return not unused.contains(index);
};

} // namespace gnev::gl::texture