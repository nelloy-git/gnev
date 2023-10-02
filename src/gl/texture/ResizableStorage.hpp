#pragma once

#include "gl/Texture.hpp"
#include "gl/texture/Image.hpp"

namespace gnev::gl::texture {

class EXPORT ResizableStorage : public Texture {
public:
    ResizableStorage(const Ctx& ctx,
                     std::size_t levels,
                     GLenum internalformat,
                     std::size_t width,
                     std::size_t height,
                     std::size_t initial_capacity);
    ResizableStorage(const ResizableStorage& other) = delete;
    ResizableStorage(ResizableStorage&& other) = default;
    virtual ~ResizableStorage();

    void setElement(std::size_t pos, const Image& img);
    void copyElement(std::size_t src, std::size_t dst, std::size_t level);
    Image getElement(std::size_t pos,
                     std::size_t level,
                     GLenum format,
                     GLenum type) const;

    void copyRange(std::size_t src, std::size_t dst, std::size_t count);

    std::size_t getLevels() const;
    GLenum getLevelInternalFormat(std::size_t level) const;
    std::size_t getLevelWidth(std::size_t level) const;
    std::size_t getLevelHeight(std::size_t level) const;
    std::size_t getLevelBufferSize(std::size_t level, GLenum format, GLenum type) const;

    void setCapacity(std::size_t capacity);
    std::size_t getCapacity() const;

    std::size_t getMaxLayers() const;

private:
    std::size_t levels;
    std::size_t capacity;

    GLenum selectFormat(GLenum internal_format) const;
};

} // namespace gnev::gl::texture