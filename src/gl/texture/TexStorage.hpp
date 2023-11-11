#pragma once

#include "gl/Texture.hpp"
#include "gl/texture/TexStorageIterator.hpp"

namespace gnev::gl {

class EXPORT TexStorage : public Texture {
public:
    TexStorage(GLuint levels,
               GLuint width,
               GLuint height,
               GLuint capacity,
               GLenum internal_format);
    TexStorage(const TexStorage& other) = delete;
    TexStorage(TexStorage&& other) = default;
    virtual ~TexStorage();

    TexStorageIterator operator[](GLuint index);
    const TexStorageIterator operator[](GLuint index) const;

    GLuint getLevels() const;
    GLuint getLevelWidth(GLuint level) const;
    GLuint getLevelHeight(GLuint level) const;
    GLuint getImageBufferSize(const TexImageInfo& info) const;
    GLuint getCapacity() const;
    GLuint getMaxCapacity() const;

private:
    GLuint levels;
    GLuint capacity;

    static GLuint getComponents(GLenum format);
    static double getBytesPerElement(GLenum type);
};

} // namespace gnev::gl