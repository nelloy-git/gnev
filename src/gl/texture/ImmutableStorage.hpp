#pragma once

#include "gl/Texture.hpp"
#include "gl/texture/ImmutableStorageIterator.hpp"

namespace gnev::gl::texture {

class EXPORT ImmutableStorage : public Texture {
public:
    ImmutableStorage(GLuint levels,
                     GLuint width,
                     GLuint height,
                     GLuint capacity,
                     GLenum internal_format);
    ImmutableStorage(const ImmutableStorage& other) = delete;
    ImmutableStorage(ImmutableStorage&& other) = default;
    virtual ~ImmutableStorage();

    ImmutableStorageIterator operator[](GLuint index);
    const ImmutableStorageIterator operator[](GLuint index) const;

    GLuint getLevels() const;
    GLuint getLevelWidth(GLuint level) const;
    GLuint getLevelHeight(GLuint level) const;
    GLuint getImageBufferSize(const ImageInfo& info) const;
    GLuint getCapacity() const;
    GLuint getMaxCapacity() const;

private:
    GLuint levels;
    GLuint capacity;

    static GLuint getComponents(GLenum format);
    static double getBytesPerElement(GLenum type);
};

} // namespace gnev::gl::texture