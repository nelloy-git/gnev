#include "gl/texture/ImmutableStorage.hpp"

#include <stdexcept>

#include "gl/Texture.hpp"
#include "gl/texture/Image.hpp"

namespace gnev::gl::texture {

ImmutableStorage::ImmutableStorage(std::size_t levels,
                                   std::size_t width,
                                   std::size_t height,
                                   std::size_t capacity,
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

ImmutableStorageIterator ImmutableStorage::operator[](std::size_t index) {
    return ImmutableStorageIterator(*this, index);
}

const ImmutableStorageIterator ImmutableStorage::operator[](std::size_t index) const {
    return ImmutableStorageIterator(*this, index);
}

std::size_t ImmutableStorage::getLevels() const { return levels; }

std::size_t ImmutableStorage::getCapacity() const { return capacity; }

std::size_t ImmutableStorage::getMaxCapacity() const {
    GLint layers;
    Ctx::Get().glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &layers);
    return layers;
}

} // namespace gnev::gl::texture