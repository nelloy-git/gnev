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
    auto required_buffer_size = getImageBufferSize(info);

    if (info.level + 1 > getLevels() ||
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
    ImageData dst_data(getImageBufferSize(info));
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
    auto required_buffer_size = getImageBufferSize(info);

    if (info.level + 1 > getLevels() ||
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
    return getStorage().getLevelWidth(level);
}

GLuint ImmutableStorageIterator::getLevelHeight(GLuint level) const {
    return getStorage().getLevelHeight(level);
}

GLuint ImmutableStorageIterator::getImageBufferSize(const ImageInfo& info) const {
    return getStorage().getImageBufferSize(info);
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

} // namespace gnev::gl::texture