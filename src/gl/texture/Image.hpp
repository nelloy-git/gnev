#pragma once

#include <memory>

#include "gl/Texture.hpp"

namespace gnev::gl::texture {

struct ImageInfo {
    GLuint level = 0;
    GLuint x = 0;
    GLuint y = 0;
    GLuint width = 0;
    GLuint height = 0;
    GLenum format;
    GLenum type;
};

struct ImageData {
    ImageData() = default;

    ImageData(std::size_t size)
        : buffer_size(size)
        , buffer(new GLubyte[size]) {}

    ImageData(std::size_t size, const std::shared_ptr<void> buffer)
        : buffer_size(size)
        , buffer(buffer) {}

    std::size_t size() const { return buffer_size; };

    void* data() { return buffer.get(); }
    const void* data() const { return buffer.get(); }

private:
    std::size_t buffer_size = 0;
    std::shared_ptr<void> buffer = nullptr;
};

struct Image {
    Image() {}

    Image(const ImageInfo& info, std::size_t size)
        : info(info)
        , data(size) {}

    Image(const ImageInfo& info, const ImageData& data)
        : info(info)
        , data(data) {}

    ImageInfo info;
    ImageData data;
};

} // namespace gnev::gl::texture