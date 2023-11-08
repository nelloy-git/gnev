#include "gl/texture/ImmutableStorage.hpp"

#include <stdexcept>

#include "gl/Texture.hpp"
#include "gl/texture/Image.hpp"

namespace gnev::gl::texture {

ImmutableStorage::ImmutableStorage(GLuint levels,
                                   GLuint width,
                                   GLuint height,
                                   GLuint capacity,
                                   GLenum internal_format)
    : Texture(GL_TEXTURE_2D_ARRAY)
    , levels(levels)
    , capacity(capacity) {
    if (capacity >= getMaxCapacity()) {
        throw std::out_of_range("");
    }

    initStorage3D(levels, internal_format, width, height, capacity);
}

ImmutableStorage::~ImmutableStorage() {}

ImmutableStorageIterator ImmutableStorage::operator[](GLuint index) {
    return ImmutableStorageIterator(*this, index);
}

const ImmutableStorageIterator ImmutableStorage::operator[](GLuint index) const {
    return ImmutableStorageIterator(*this, index);
}

GLuint ImmutableStorage::getLevels() const { return levels; }

GLuint ImmutableStorage::getLevelWidth(GLuint level) const {
    GLint width;
    getLevelParameteriv(level, GL_TEXTURE_WIDTH, &width);
    return width;
}

GLuint ImmutableStorage::getLevelHeight(GLuint level) const {
    GLint height;
    getLevelParameteriv(level, GL_TEXTURE_HEIGHT, &height);
    return height;
}

GLuint ImmutableStorage::getImageBufferSize(const ImageInfo& info) const {
    return (info.width > 0 ? info.width : getLevelWidth(info.level) - info.x) *
           (info.height > 0 ? info.height : getLevelHeight(info.level) - info.y) *
           getComponents(info.format) * getBytesPerElement(info.type);
}

GLuint ImmutableStorage::getCapacity() const { return capacity; }

GLuint ImmutableStorage::getMaxCapacity() const {
    GLint layers;
    Ctx::Get().glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &layers);
    return layers;
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

} // namespace gnev::gl::texture