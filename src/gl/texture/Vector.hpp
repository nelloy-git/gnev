#pragma once

#include "gl/texture/ResizableStorage.hpp"

namespace gnev::gl::texture {

class EXPORT Vector : public ResizableStorage {
public:
    static constexpr float CAP_MULT = 2;

    Vector(const Ctx& ctx,
           std::size_t levels,
           GLenum internalformat,
           std::size_t width,
           std::size_t height,
           std::size_t initial_capacity);
    Vector(const Vector& other) = delete;
    Vector(Vector&& other) = default;
    virtual ~Vector();

    void setElement(std::size_t pos, const Image& img);
    void copyElement(std::size_t src, std::size_t dst, std::size_t level);
    Image getElement(std::size_t pos,
                     std::size_t level,
                     GLenum format,
                     GLenum type) const;
    void insertElement(std::size_t pos, const Image& img);
    void pushElementBack(const Image& img);
    void eraseElement(std::size_t pos);

    std::size_t getSize() const;
    void reserve(std::size_t capacity);
    void shrinkToFit();
    void clear();

private:
    std::size_t size;
};

} // namespace gnev::gl::texture