#pragma once

#include "gl/Ctx.hpp"

namespace gnev::gl {

class Buffer;

template <IsTriviallyCopyable T>
class EXPORT BufAccessorConst {
public:
    BufAccessorConst(Buffer& buffer, GLuint index);
    virtual ~BufAccessorConst();

    GLuint getIndex() const;
    const Buffer& getBuffer() const;

    virtual void getData(T& dst) const = 0;
    virtual void copyTo(void* dst, std::size_t gl_offset, std::size_t n_bytes) const = 0;

private:
    const Buffer& buffer;
    GLuint index;
};

template <IsTriviallyCopyable T>
BufAccessorConst<T>::BufAccessorConst(Buffer& buffer, GLuint index)
    : buffer(buffer)
    , index(index) {}

template <IsTriviallyCopyable T>
BufAccessorConst<T>::~BufAccessorConst<T>() {}

template <IsTriviallyCopyable T>
GLuint BufAccessorConst<T>::getIndex() const {
    return index;
}

template <IsTriviallyCopyable T>
const Buffer& BufAccessorConst<T>::getBuffer() const {
    return buffer;
}

}