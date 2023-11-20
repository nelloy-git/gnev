#include "material/base/MaterialTexStorage.hpp"

#include "gl/texture/TexStorageIterator.hpp"
#include "material/base/Define.hpp"

namespace gnev::base {

MaterialTexStorage::MaterialTexStorage(GLuint capacity,
                                       GLuint levels,
                                       GLuint width,
                                       GLuint height,
                                       GLenum internal_format,
                                       const gl::TexStorageIndexMap::CleanUp& clean_up)
    : storage(levels, width, height, capacity, internal_format, clean_up) {}

MaterialTexStorage::~MaterialTexStorage() {}

std::optional<MaterialTexIndex> MaterialTexStorage::initIndex() {
    return storage.initUnusedIndex();
}

void MaterialTexStorage::freeIndex(MaterialTexIndex index) { storage.freeIndex(index); }

gl::TexStorageIterator MaterialTexStorage::at(GLuint index) { return storage.at(index); }

const gl::TexStorageIterator MaterialTexStorage::at(GLuint index) const {
    return storage.at(index);
}

} // namespace gnev::base