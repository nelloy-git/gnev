#pragma once

#include "gl/Ctx.hpp"

namespace gnev::gl {

class Buffer;

template <IsTriviallyCopyable T>
class EXPORT BufAccessor {
public:
    BufAccessor(Buffer& buffer, GLuint index);
    virtual ~BufAccessor();

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
BufAccessor<T>::BufAccessor(Buffer& buffer, GLuint index)
    : buffer(buffer)
    , index(index) {}

template <IsTriviallyCopyable T>
BufAccessor<T>::~BufAccessor<T>() {}

template <IsTriviallyCopyable T>
GLuint BufAccessor<T>::getIndex() const {
    return index;
}

template <IsTriviallyCopyable T>
Buffer& BufAccessor<T>::getBuffer() const {
    return buffer;
}

}