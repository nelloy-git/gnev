#pragma once

#include "gl/Ctx.hpp"

namespace gnev::gl {

class Buffer;

template <IsTriviallyCopyable T>
class EXPORT BufIterator {
public:
    BufIterator(Buffer& buffer, GLuint index);
    virtual ~BufIterator();

    GLuint getIndex() const;
    Buffer& getBuffer() const;

    virtual void getData(T& dst) const = 0;
    virtual void copyTo(void* dst, std::size_t gl_offset, std::size_t n_bytes) const = 0;

    virtual void setData(const T&) const = 0;
    virtual void copyFrom(const void* src, std::size_t gl_offset, std::size_t n_bytes) const = 0;

private:
    Buffer& buffer;
    GLuint index;
};

template <IsTriviallyCopyable T>
BufIterator<T>::BufIterator(Buffer& buffer, GLuint index)
    : buffer(buffer)
    , index(index) {}

template <IsTriviallyCopyable T>
BufIterator<T>::~BufIterator<T>() {}

template <IsTriviallyCopyable T>
GLuint BufIterator<T>::getIndex() const {
    return index;
}

template <IsTriviallyCopyable T>
Buffer& BufIterator<T>::getBuffer() const {
    return buffer;
}
} // namespace gnev::gl