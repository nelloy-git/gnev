#include "gl/texture/TexStorageIterator.hpp"

#include "gl/texture/TexStorage.hpp"

namespace gnev::gl {

TexStorageIterator::TexStorageIterator(TexStorage& storage, GLuint index)
    : storage(std::ref(storage))
    , index(index) {}

TexStorageIterator::TexStorageIterator(const TexStorage& storage, GLuint index)
    : storage(std::ref(storage))
    , index(index) {}

TexStorageIterator::~TexStorageIterator() {}

GLuint TexStorageIterator::getIndex() const { return index; }

TexImage TexStorageIterator::getImage(const TexImageInfo& info,
                                   const TexImageData& data) const {
    auto required_buffer_size = getImageBufferSize(info);

    if (info.level + 1 > getLevels() || info.x + info.width > getLevelWidth(info.level) ||
        info.y + info.height > getLevelHeight(info.level) ||
        data.size() < required_buffer_size) {
        throw std::out_of_range("");
    }

    TexImage dst(info, data);
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

TexImage TexStorageIterator::getImage(const TexImageInfo& info) const {
    TexImageData dst_data(getImageBufferSize(info));
    return getImage(info, dst_data);
}

TexImage TexStorageIterator::getImage(GLuint level, GLenum format, GLenum type) const {
    TexImageInfo info{.level = level,
                      .x = 0,
                      .y = 0,
                      .width = getLevelWidth(level),
                      .height = getLevelHeight(level),
                      .format = format,
                      .type = type};
    return getImage(info);
}

void TexStorageIterator::getImage(TexImage& dst) const {
    dst = getImage(dst.info, dst.data);
}

void TexStorageIterator::setImage(const TexImage& src) {
    auto info = src.info;
    auto required_buffer_size = getImageBufferSize(info);

    if (info.level + 1 > getLevels() || info.x + info.width > getLevelWidth(info.level) ||
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

GLuint TexStorageIterator::getLevels() const { return getStorage().getLevels(); }

GLenum TexStorageIterator::getLevelInternalFormat(GLuint level) const {
    GLenum internal_format;
    getStorage().getLevelParameteriv(level,
                                     GL_TEXTURE_INTERNAL_FORMAT,
                                     reinterpret_cast<GLint*>(&internal_format));
    return internal_format;
}

GLuint TexStorageIterator::getLevelWidth(GLuint level) const {
    return getStorage().getLevelWidth(level);
}

GLuint TexStorageIterator::getLevelHeight(GLuint level) const {
    return getStorage().getLevelHeight(level);
}

GLuint TexStorageIterator::getImageBufferSize(const TexImageInfo& info) const {
    return getStorage().getImageBufferSize(info);
}

TexStorage& TexStorageIterator::getStorage() {
    return std::get<reference<TexStorage>>(storage);
}

const TexStorage& TexStorageIterator::getStorage() const {
    switch (storage.index()) {
    case 0:
        return std::get<0>(storage);
    case 1:
        return std::get<1>(storage);
    default:
        throw std::out_of_range("");
    }
}

} // namespace gnev::gl