#pragma once

#include "gl/texture/TexStorageIndexMap.hpp"
#include "gl/texture/TexStorageIterator.hpp"
#include "material/base/Define.hpp"

namespace gnev::base {

class EXPORT MaterialTexStorage {
public:
    MaterialTexStorage(GLuint capacity,
                       GLuint levels,
                       GLuint width,
                       GLuint height,
                       GLenum internal_format,
                       const gl::TexStorageIndexMap::CleanUp& clean_up = std::nullopt);
    virtual ~MaterialTexStorage();

    std::optional<MaterialTexIndex> initIndex();
    void freeIndex(MaterialTexIndex);

    gl::TexStorageIterator at(GLuint index);
    const gl::TexStorageIterator at(GLuint index) const;

private:
    gl::TexStorageIndexMap storage;
};

} // namespace gnev::base