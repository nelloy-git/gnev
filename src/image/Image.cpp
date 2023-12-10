#include "image/Image.hpp"

namespace gnev {

GLuint ImageInfo::getTextureBufferSize() const {
    using enum ImageFormat;
    using enum ImageType;
    return width * height * getTextureBufferSize();
}

GLuint ImageInfo::getBytesPerPixel() const {
    using enum ImageFormat;

    switch (type) {
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
        return getComponents() * sizeof(GLubyte);
    case BYTE:
        return getComponents() * sizeof(GLbyte);
    case UNSIGNED_SHORT:
        return getComponents() * sizeof(GLushort);
    case SHORT:
        return getComponents() * sizeof(GLshort);
    case UNSIGNED_INT:
        return getComponents() * sizeof(GLuint);
    case INT:
        return getComponents() * sizeof(GLint);
    case FLOAT:
        return getComponents() * sizeof(GLfloat);
    }
}

GLuint ImageInfo::getComponents() const {
    using enum ImageType;

    GLuint bytes_per_pixel = 0;
    switch (format) {
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
    }
}

} // namespace gnev