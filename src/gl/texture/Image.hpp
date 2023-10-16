#pragma once

#include <memory>

#include "gl/Texture.hpp"

namespace gnev::gl::texture {

struct ImagePoint {
    GLuint level = 0;
    GLuint x = 0;
    GLuint y = 0;
    GLuint z = 0;
};

struct ImageRect : public ImagePoint {
    GLuint width = 0;
    GLuint height = 0;
};

struct ImageInfo : public ImageRect {
    GLenum format;
    GLenum type;
};

struct ImageData {
    ImageData() {}

    ImageData(std::size_t size)
        : size(size)
        , buffer(new GLubyte[size]) {}

    ImageData(std::size_t size,
              void* p_buffer)
        : size(size)
        , buffer(static_cast<GLubyte*>(p_buffer)) {}

    std::size_t size = 0;
    GLubyte* buffer = nullptr;
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