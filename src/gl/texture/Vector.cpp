#include "gl/texture/Vector.hpp"

#include <stdexcept>

using namespace gnev::gl;
using namespace gnev::gl::texture;

Vector::Vector(const Ctx& ctx,
               std::size_t levels,
               GLenum internalformat,
               std::size_t width,
               std::size_t height,
               std::size_t initial_capacity)
    : ResizableStorage(ctx, levels, internalformat, width, height, initial_capacity)
    , size(0) {}

Vector::~Vector() {}

void Vector::setElement(std::size_t pos, const Image& img) {
    if (pos >= size) {
        throw std::out_of_range("");
    }

    ResizableStorage::setElement(pos, img);
}

void Vector::copyElement(std::size_t src, std::size_t dst, std::size_t level) {
    if (src >= size || dst >= size) {
        throw std::out_of_range("");
    }

    ResizableStorage::copyElement(src, dst, level);
}

Image Vector::getElement(std::size_t pos,
                         std::size_t level,
                         GLenum format,
                         GLenum type) const {
    if (pos >= size) {
        throw std::out_of_range("");
    }

    return ResizableStorage::getElement(pos, level, format, type);
}

void Vector::insertElement(std::size_t pos, const Image& img) {
    if (pos > size) {
        throw std::out_of_range("");
    }

    while (size + 1 > getCapacity()) {
        setCapacity(CAP_MULT * getCapacity());
    }

    if (pos != size) {
        ResizableStorage::copyRange(pos, pos + 1, size - pos);
    }
    ResizableStorage::setElement(pos, img);
    size += 1;
}

void Vector::pushElementBack(const Image& img) { insertElement(size, img); }

void Vector::eraseElement(std::size_t pos) {
    if (pos >= size) {
        throw std::out_of_range("");
    }

    if (pos + 1 != size) {
        ResizableStorage::copyRange(pos + 1, pos, size - (pos + 1));
    }
    size -= 1;
}

std::size_t Vector::getSize() const { return size; }

void Vector::reserve(std::size_t cap) {
    if (cap <= ResizableStorage::getCapacity()) {
        return;
    }
    ResizableStorage::setCapacity(cap);
}

void Vector::shrinkToFit() { ResizableStorage::setCapacity(size); }

void Vector::clear() { size = 0; }