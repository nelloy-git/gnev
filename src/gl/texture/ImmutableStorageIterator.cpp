#include "gl/texture/ImmutableStorageIterator.hpp"

#include "gl/texture/ImmutableStorage.hpp"

namespace gnev::gl::texture {

ImmutableStorageIterator::ImmutableStorageIterator(ImmutableStorage& storage,
                                                   GLuint index)
    : storage(std::ref(storage))
    , index(index) {}

ImmutableStorageIterator::ImmutableStorageIterator(const ImmutableStorage& storage,
                                                   GLuint index)
    : storage(std::ref(storage))
    , index(index) {}

ImmutableStorageIterator::~ImmutableStorageIterator() {}

GLuint ImmutableStorageIterator::getIndex() const { return index; }

Image ImmutableStorageIterator::getImage(const ImageInfo& info,
                                         const ImageData& data) const {
    auto required_buffer_size = getBufferSize(info);

    if (info.level + 1 >= getLevels() ||
        info.x + info.width > getLevelWidth(info.level) ||
        info.y + info.height > getLevelHeight(info.level) ||
        data.size() < required_buffer_size) {
        throw std::out_of_range("");
    }

    Image dst(info, data);
    getStorage().getSubImage(info.level,
                             info.x,
                             info.y,
                             index,
                             info.width,
                             info.height,
                             1,
                             info.format,
                             info.type,
                             dst.data.size(),
                             dst.data.data());

    return dst;
}

Image ImmutableStorageIterator::getImage(const ImageInfo& info) const {
    ImageData dst_data(getBufferSize(info));
    return getImage(info, dst_data);
}

Image ImmutableStorageIterator::getImage(GLuint level, GLenum format, GLenum type) const {
    ImageInfo info{.level = level,
                   .x = 0,
                   .y = 0,
                   .width = getLevelWidth(level),
                   .height = getLevelHeight(level),
                   .format = format,
                   .type = type};
    return getImage(info);
}

void ImmutableStorageIterator::getImage(Image& dst) const {
    dst = getImage(dst.info, dst.data);
}

void ImmutableStorageIterator::setImage(const Image& src) {
    auto info = src.info;
    auto required_buffer_size = getBufferSize(info);

    if (info.level + 1 >= getLevels() ||
        info.x + info.width > getLevelWidth(info.level) ||
        info.y + info.height > getLevelHeight(info.level) ||
        src.data.size() < required_buffer_size) {
        throw std::out_of_range("");
    }

    getStorage().setSubImage3D(info.level,
                               info.x,
                               info.y,
                               index,
                               info.width,
                               info.height,
                               1,
                               info.format,
                               info.type,
                               src.data.data());
}

GLuint ImmutableStorageIterator::getLevels() const { return getStorage().getLevels(); }

GLenum ImmutableStorageIterator::getLevelInternalFormat(GLuint level) const {
    GLenum internal_format;
    getStorage().getLevelParameteriv(level,
                                     GL_TEXTURE_INTERNAL_FORMAT,
                                     reinterpret_cast<GLint*>(&internal_format));
    return internal_format;
}

GLuint ImmutableStorageIterator::getLevelWidth(GLuint level) const {
    GLint width;
    getStorage().getLevelParameteriv(level, GL_TEXTURE_WIDTH, &width);
    return width;
}

GLuint ImmutableStorageIterator::getLevelHeight(GLuint level) const {
    GLint height;
    getStorage().getLevelParameteriv(level, GL_TEXTURE_HEIGHT, &height);
    return height;
}

GLuint ImmutableStorageIterator::getBufferSize(const ImageInfo& info) const {
    return (info.width > 0 ? info.width : getLevelWidth(info.level) - info.x) *
           (info.height > 0 ? info.height : getLevelHeight(info.level) - info.y) *
           getComponents(info.format) * getBytesPerElement(info.type);
}

ImmutableStorage& ImmutableStorageIterator::getStorage() {
    return std::get<reference<ImmutableStorage>>(storage);
}

const ImmutableStorage& ImmutableStorageIterator::getStorage() const {
    switch (storage.index()) {
    case 0:
        return std::get<0>(storage);
    case 1:
        return std::get<1>(storage);
    default:
        throw std::out_of_range("");
    }
}

GLuint ImmutableStorageIterator::getComponents(GLenum format) {
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

double ImmutableStorageIterator::getBytesPerElement(GLenum type) {
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