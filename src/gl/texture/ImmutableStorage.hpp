#pragma once

#include "gl/Texture.hpp"
#include "gl/texture/Image.hpp"

namespace gnev::gl::texture {

class EXPORT ImmutableStorage : public Texture {
public:
    ImmutableStorage(std::size_t width,
                     std::size_t height,
                     GLenum internal_format,
                     std::size_t capacity,
                     std::size_t levels);
    ImmutableStorage(const ImmutableStorage& other) = delete;
    ImmutableStorage(ImmutableStorage&& other) = default;
    virtual ~ImmutableStorage();

    Image getImage(ImageInfo& info) const;
    void getImage(Image& dst) const;

    void setImage(const Image& img);

    std::size_t getLevels() const;
    GLenum getLevelInternalFormat(std::size_t level) const;
    std::size_t getLevelWidth(std::size_t level) const;
    std::size_t getLevelHeight(std::size_t level) const;

    std::size_t getCapacity() const;
    std::size_t getMaxLayers() const;
    std::size_t getBufferSize(const ImageInfo& info) const;

private:
    std::size_t levels;
    std::size_t capacity;

    void applyImageSize(ImageInfo& info) const;

    static GLenum selectFormat(GLenum internal_format);
    static GLuint getElements(GLenum format);
    static double getBytesPerElement(GLenum type);
};

} // namespace gnev::gl::texture