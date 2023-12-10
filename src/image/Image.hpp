#pragma once

#include "gl/Ctx.hpp"
#include "util/Export.hpp"
#include "util/Ref.hpp"

namespace gnev {

enum ImageFormat : GLenum {
    RED = GL_RED,
    RG = GL_RG,
    RGB = GL_RGB,
    BGR = GL_BGR,
    RGBA = GL_RGBA,
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    STENCIL_INDEX = GL_STENCIL_INDEX
};

constexpr GLenum toGLenum(ImageFormat format) { return static_cast<GLenum>(format); }

enum ImageType : GLenum {
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    BYTE = GL_BYTE,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    SHORT = GL_SHORT,
    UNSIGNED_INT = GL_UNSIGNED_INT,
    INT = GL_INT,
    FLOAT = GL_FLOAT,
    UNSIGNED_BYTE_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
    UNSIGNED_BYTE_2_3_3_REV = GL_UNSIGNED_BYTE_2_3_3_REV,
    UNSIGNED_SHORT_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
    UNSIGNED_SHORT_5_6_5_REV = GL_UNSIGNED_SHORT_5_6_5_REV,
    UNSIGNED_SHORT_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
    UNSIGNED_SHORT_4_4_4_4_REV = GL_UNSIGNED_SHORT_4_4_4_4_REV,
    UNSIGNED_SHORT_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
    UNSIGNED_SHORT_1_5_5_5_REV = GL_UNSIGNED_SHORT_1_5_5_5_REV,
    UNSIGNED_INT_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
    UNSIGNED_INT_8_8_8_8_REV = GL_UNSIGNED_INT_8_8_8_8_REV,
    UNSIGNED_INT_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
    UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV
};

constexpr GLenum toGLenum(ImageType type) { return static_cast<GLenum>(type); }

struct EXPORT ImageInfo {
    GLuint level = 0;
    GLuint x = 0;
    GLuint y = 0;
    GLuint width = 0;
    GLuint height = 0;
    ImageFormat format;
    ImageType type;

    GLuint getTextureBufferSize() const;
    GLuint getBytesPerPixel() const;
    GLuint getComponents() const;
};

struct ImageData {
    ImageData(std::size_t size)
        : buffer_size(size)
        , buffer(std::make_shared<GLubyte[]>(size)) {}

    ImageData(std::size_t size, const std::shared_ptr<GLubyte[]> buffer)
        : buffer_size(size)
        , buffer(buffer) {}

    template <typename T = void>
    T* get() const {
        return buffer.getPtr().get();
    }

    const std::size_t buffer_size;
    const Ref<GLubyte[]> buffer;
};

struct Image {
    Image(const ImageInfo& info, std::size_t size)
        : info(info)
        , data(size) {}

    Image(const ImageInfo& info, const ImageData& data)
        : info(info)
        , data(data) {}

    const ImageInfo info;
    const ImageData data;
};

} // namespace gnev