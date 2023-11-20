#include "gl/texture/TexStorageIndexMap.hpp"

namespace gnev::gl {

TexStorageIndexMap::TexStorageIndexMap(GLuint levels,
                                       GLuint width,
                                       GLuint height,
                                       GLuint capacity,
                                       GLenum internal_format,
                                       CleanUp clean_up)
    : TexStorage(levels, width, height, capacity, internal_format)
    , clean_up(clean_up) {
    for (GLuint i = 0; i < capacity; ++i) {
        unused.insert(unused.end(), capacity - i - 1);
    }
}

TexStorageIndexMap::~TexStorageIndexMap() {}

TexStorageIterator TexStorageIndexMap::at(GLuint index) {
    if (unused.contains(index)) {
        throw std::out_of_range("");
    }
    return TexStorage::operator[](index);
}

const TexStorageIterator TexStorageIndexMap::at(GLuint index) const {
    if (unused.contains(index)) {
        throw std::out_of_range("");
    }
    return TexStorage::operator[](index);
}

std::optional<GLuint> TexStorageIndexMap::initUnusedIndex() {
    auto iter = unused.begin();
    if (iter == unused.end()) {
        return std::nullopt;
    }
    GLuint index = *iter;
    unused.erase(iter);
    return index;
}

void TexStorageIndexMap::freeIndex(GLuint index) {
    unused.insert(unused.end(), index);
    if (clean_up.has_value()) {
        clean_up.value()(*this, index);
    }
}

bool TexStorageIndexMap::containsIndex(const GLuint& index) const {
    return not unused.contains(index);
};

} // namespace gnev::gl