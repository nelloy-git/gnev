#pragma once

#include "gl/texture/TexStorageIndexMap.hpp"
#include "material/base/Define.hpp"

namespace gnev::base {

class EXPORT MaterialTexRefStorage {
public:
    MaterialTexRefStorage(GLuint capacity,
                          GLuint levels,
                          GLuint width,
                          GLuint height,
                          GLenum internal_format,
                          const gl::TexStorageIndexMap::CleanUp& clean_up = std::nullopt);
    virtual ~MaterialTexRefStorage();

    std::optional<MaterialTexIndex> initIndex();
    void freeIndex(MaterialTexIndex);

private:
    gl::TexStorageIndexMap storage;
};

} // namespace gnev::base