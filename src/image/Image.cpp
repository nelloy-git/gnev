#include "image/Image.hpp"

namespace gnev {

namespace details {

GLuint getComponents(TextureFormat format) {

    GLuint bytes_per_pixel = 0;
    switch (format) {
        using enum TextureFormat;
    case RED:
    case DEPTH_COMPONENT:
    case STENCIL_INDEX:
        return 1;
    case RG:
        return 2;
    case RGB:
        return 3;
    case BGR:
        return 3;
    case RGBA:
        return 4;
    default:
        throw std::out_of_range("");
    }
}

GLuint getBytesPerPixel(TextureType type, TextureFormat format) {
    switch (type) {
        using enum TextureType;
    case UNSIGNED_BYTE_3_3_2:
    case UNSIGNED_BYTE_2_3_3_REV:
        return sizeof(GLubyte);
    case UNSIGNED_SHORT_5_6_5:
    case UNSIGNED_SHORT_5_6_5_REV:
    case UNSIGNED_SHORT_4_4_4_4:
    case UNSIGNED_SHORT_4_4_4_4_REV:
    case UNSIGNED_SHORT_5_5_5_1:
    case UNSIGNED_SHORT_1_5_5_5_REV:
        return sizeof(GLushort);
    case UNSIGNED_INT_8_8_8_8:
    case UNSIGNED_INT_8_8_8_8_REV:
    case UNSIGNED_INT_10_10_10_2:
    case UNSIGNED_INT_2_10_10_10_REV:
        return sizeof(GLuint);

    case UNSIGNED_BYTE:
        return getComponents(format) * sizeof(GLubyte);
    case BYTE:
        return getComponents(format) * sizeof(GLbyte);
    case UNSIGNED_SHORT:
        return getComponents(format) * sizeof(GLushort);
    case SHORT:
        return getComponents(format) * sizeof(GLshort);
    case UNSIGNED_INT:
        return getComponents(format) * sizeof(GLuint);
    case INT:
        return getComponents(format) * sizeof(GLint);
    case FLOAT:
        return getComponents(format) * sizeof(GLfloat);
    default:
        throw std::out_of_range("");
    }
}

} // namespace details

GLuint ImageInfo::calcTextureBufferSize() const {
    return width * height * getBytesPerPixel();
}

GLuint ImageInfo::getBytesPerPixel() const {
    return details::getBytesPerPixel(type, format);
}

GLuint ImageInfo::getComponents() const { return details::getComponents(format); }

ImageInfo3d::ImageInfo3d(GLuint z, const ImageInfo& info)
    : level(info.level)
    , x(info.x)
    , y(info.y)
    , z(z)
    , width(info.width)
    , height(info.height)
    , depth(1)
    , format(info.format)
    , type(info.type) {}

GLuint ImageInfo3d::calcTextureBufferSize() const {
    return depth * width * height * getBytesPerPixel();
}

GLuint ImageInfo3d::getBytesPerPixel() const {
    return details::getBytesPerPixel(type, format);
}

GLuint ImageInfo3d::getComponents() const { return details::getComponents(format); }

} // namespace gnev