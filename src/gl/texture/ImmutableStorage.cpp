#include "gl/texture/ImmutableStorage.hpp"

#include <stdexcept>

#include "gl/Texture.hpp"
#include "gl/texture/Image.hpp"

namespace gnev::gl::texture {

ImmutableStorage::ImmutableStorage(std::size_t width,
                                   std::size_t height,
                                   GLenum internal_format,
                                   std::size_t capacity,
                                   std::size_t levels)
    : Texture(GL_TEXTURE_2D_ARRAY)
    , levels(levels)
    , capacity(capacity) {
    if (capacity >= getMaxLayers()) {
        throw std::out_of_range("");
    }

    initStorage3D(levels, internal_format, width, height, capacity);
}

ImmutableStorage::~ImmutableStorage() {}

Image ImmutableStorage::getImage(ImageInfo& info) const {
    applyImageSize(info);
    if (info.level + 1 >= getLevels() ||
        info.x + info.width > getLevelWidth(info.level) ||
        info.y + info.height > getLevelHeight(info.level) || info.z >= capacity) {
        throw std::out_of_range("");
    }

    Image dst(info, getBufferSize(info));

    getSubImage(info.level,
                info.x,
                info.y,
                info.z,
                info.width,
                info.height,
                1,
                info.format,
                info.type,
                dst.data.size,
                dst.data.buffer.get());

    return dst;
}

void ImmutableStorage::getImage(Image& dst) const {
    auto& info = dst.info;

    applyImageSize(info);
    auto required_buffer_size = getBufferSize(dst.info);
    if (info.level + 1 >= getLevels() ||
        info.x + info.width > getLevelWidth(info.level) ||
        info.y + info.height > getLevelHeight(info.level) || info.z >= capacity ||
        dst.data.size < required_buffer_size) {
        throw std::out_of_range("");
    }

    getSubImage(info.level,
                info.x,
                info.y,
                info.z,
                info.width,
                info.height,
                1,
                info.format,
                info.type,
                dst.data.size,
                dst.data.buffer.get());
}

void ImmutableStorage::setImage(const Image& dst) {
    auto info = dst.info;

    applyImageSize(info);
    auto required_buffer_size = getBufferSize(dst.info);
    if (info.level + 1 >= getLevels() ||
        info.x + info.width > getLevelWidth(info.level) ||
        info.y + info.height > getLevelHeight(info.level) || info.z >= capacity ||
        dst.data.size < required_buffer_size) {
        throw std::out_of_range("");
    }

    setSubImage3D(info.level,
                  info.x,
                  info.y,
                  info.z,
                  info.width,
                  info.height,
                  1,
                  info.format,
                  info.type,
                  dst.data.buffer.get());
}

std::size_t ImmutableStorage::getLevels() const { return levels; }

GLenum ImmutableStorage::getLevelInternalFormat(std::size_t level) const {
    GLenum internal_format;
    getLevelParameteriv(level,
                        GL_TEXTURE_INTERNAL_FORMAT,
                        reinterpret_cast<GLint*>(&internal_format));
    return internal_format;
}

std::size_t ImmutableStorage::getLevelWidth(std::size_t level) const {
    GLint width;
    getLevelParameteriv(level, GL_TEXTURE_WIDTH, &width);
    return width;
}

std::size_t ImmutableStorage::getLevelHeight(std::size_t level) const {
    GLint height;
    getLevelParameteriv(level, GL_TEXTURE_HEIGHT, &height);
    return height;
}

std::size_t ImmutableStorage::getCapacity() const { return capacity; }

std::size_t ImmutableStorage::getMaxLayers() const {
    GLint layers;
    Ctx::Get().glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &layers);
    return layers;
}

std::size_t ImmutableStorage::getBufferSize(const ImageInfo& info) const {
    return (info.width > 0 ? info.width : getLevelWidth(info.level) - info.x) *
           (info.height > 0 ? info.height : getLevelHeight(info.level) - info.y) *
           getComponents(info.format) * getBytesPerElement(info.type);
}

GLuint ImmutableStorage::getComponents(GLenum format) {
    switch (format) {
    case GL_RED:
        return 1;
    case GL_RG:
        return 2;
    case GL_RGB:
        return 3;
    case GL_BGR:
        return 3;
    case GL_RGBA:
        return 4;
    case GL_BGRA:
        return 4;
    case GL_DEPTH_COMPONENT:
        return 1;
    case GL_STENCIL_INDEX:
        return 1;
    default:
        throw std::out_of_range("");
    }
}

double ImmutableStorage::getBytesPerElement(GLenum type) {
    switch (type) {
    case GL_UNSIGNED_BYTE:
        return sizeof(GLubyte);
    case GL_BYTE:
        return sizeof(GLbyte);
    case GL_UNSIGNED_SHORT:
        return sizeof(GLushort);
    case GL_SHORT:
        return sizeof(GLshort);
    case GL_UNSIGNED_INT:
        return sizeof(GLuint);
    case GL_INT:
        return sizeof(GLint);
    case GL_FLOAT:
        return sizeof(GLfloat);
    case GL_UNSIGNED_BYTE_3_3_2:
    case GL_UNSIGNED_BYTE_2_3_3_REV:
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_5_6_5_REV:
        return double(sizeof(GLushort)) / 3;
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_4_4_4_4_REV:
    case GL_UNSIGNED_SHORT_5_5_5_1:
    case GL_UNSIGNED_SHORT_1_5_5_5_REV:
        return double(sizeof(GLushort)) / 4;
    case GL_UNSIGNED_INT_8_8_8_8:
    case GL_UNSIGNED_INT_8_8_8_8_REV:
    case GL_UNSIGNED_INT_10_10_10_2:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
        return double(sizeof(GLuint)) / 4;
    default:
        throw std::out_of_range("");
    }
}

void ImmutableStorage::applyImageSize(ImageInfo& info) const {
    if (info.width == 0) {
        info.width = getLevelWidth(info.level) - info.x;
    }

    if (info.height == 0) {
        info.height = getLevelHeight(info.level) - info.y;
    }
}

} // namespace gnev::gl::texture