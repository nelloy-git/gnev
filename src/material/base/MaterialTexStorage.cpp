#include "material/base/Define.hpp"
#include "material/base/MaterialTexRefStorage.hpp"

namespace gnev::base {

MaterialTexRefStorage::MaterialTexRefStorage(GLuint capacity,
                                             GLuint levels,
                                             GLuint width,
                                             GLuint height,
                                             GLenum internal_format,
                                             const gl::TexStorageIndexMap::CleanUp&
                                                 clean_up)
    : storage(levels, width, height, capacity, internal_format, clean_up) {}

MaterialTexRefStorage::~MaterialTexRefStorage() {}

std::optional<MaterialTexRefIndex> MaterialTexRefStorage::initIndex() {
    return storage.initUnusedIndex();
}

void MaterialTexRefStorage::freeIndex(MaterialTexRefIndex index) {
    storage.freeIndex(index);
}

} // namespace gnev::base