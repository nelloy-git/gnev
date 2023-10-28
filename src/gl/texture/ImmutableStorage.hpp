#pragma once

#include "gl/Texture.hpp"
#include "gl/texture/ImmutableStorageIterator.hpp"

namespace gnev::gl::texture {

class EXPORT ImmutableStorage : public Texture {
public:
    ImmutableStorage(std::size_t levels,
                     std::size_t width,
                     std::size_t height,
                     std::size_t capacity,
                     GLenum internal_format);
    ImmutableStorage(const ImmutableStorage& other) = delete;
    ImmutableStorage(ImmutableStorage&& other) = default;
    virtual ~ImmutableStorage();

    ImmutableStorageIterator operator[](std::size_t index);
    const ImmutableStorageIterator operator[](std::size_t index) const;

    std::size_t getLevels() const;
    std::size_t getCapacity() const;
    std::size_t getMaxCapacity() const;

private:
    std::size_t levels;
    std::size_t capacity;

    void applyImageSize(ImageInfo& info) const;

    static GLuint getComponents(GLenum format);
    static double getBytesPerElement(GLenum type);
};

} // namespace gnev::gl::texture