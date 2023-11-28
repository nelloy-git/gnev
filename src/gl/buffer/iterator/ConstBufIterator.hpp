#pragma once

#include "gl/Ctx.hpp"

namespace gnev::gl {

class Buffer;

template <IsTriviallyCopyable T>
class EXPORT ConstBufIterator {
public:
    ConstBufIterator(const Buffer& buffer, GLuint index);
    virtual ~ConstBufIterator();

    GLuint getIndex() const;
    const Buffer& getBuffer() const;

    virtual T getData() const = 0;
    virtual void copyTo(void* dst, std::size_t gl_offset, std::size_t n_bytes) const = 0;

private:
    const Buffer& buffer;
    GLuint index;
};

template <IsTriviallyCopyable T>
ConstBufIterator<T>::ConstBufIterator(const Buffer& buffer, GLuint index)
    : buffer(buffer)
    , index(index) {}

template <IsTriviallyCopyable T>
ConstBufIterator<T>::~ConstBufIterator<T>() {}

template <IsTriviallyCopyable T>
GLuint ConstBufIterator<T>::getIndex() const {
    return index;
}

template <IsTriviallyCopyable T>
const Buffer& ConstBufIterator<T>::getBuffer() const {
    return buffer;
}

} // namespace gnev::gl