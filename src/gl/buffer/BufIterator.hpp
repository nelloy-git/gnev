#pragma once

#include <variant>

#include "gl/Buffer.hpp"

namespace gnev::gl {

template <IsTriviallyCopyable T>
class EXPORT BufIterator {
public:
    BufIterator(const BufIterator&) = delete;
    BufIterator(BufIterator&&) = default;
    virtual ~BufIterator();

    GLuint getIndex() const;

    virtual T getData() const = 0;
    virtual void setData(const T&) = 0;
    virtual void copyFrom(void* dst, std::size_t data_offset, std::size_t n) const = 0;
    virtual void copyTo(const void* src, std::size_t data_offset, std::size_t n) = 0;

    virtual T& operator*() = 0;
    virtual const T& operator*() const = 0;

    virtual T* operator->() = 0;
    virtual const T* operator->() const = 0;

protected:
    BufIterator(Buffer& buffer, GLuint index);
    BufIterator(const Buffer& buffer, GLuint index);

    Buffer& getStorage();
    const Buffer& getStorage() const;

private:
    template <typename V>
    using ref = std::reference_wrapper<V>;

    const std::variant<ref<Buffer>, ref<const Buffer>> buffer;
    GLuint index;
};

template <IsTriviallyCopyable T>
BufIterator<T>::~BufIterator() {}

template <IsTriviallyCopyable T>
GLuint BufIterator<T>::getIndex() const {
    return index;
}

template <IsTriviallyCopyable T>
BufIterator<T>::BufIterator(Buffer& buffer, GLuint index)
    : buffer(ref<Buffer>(buffer))
    , index(index) {}

template <IsTriviallyCopyable T>
BufIterator<T>::BufIterator(const Buffer& buffer, GLuint index)
    : buffer(ref<const Buffer>(buffer))
    , index(index) {}

template <IsTriviallyCopyable T>
Buffer& BufIterator<T>::getStorage() {
    return std::get<ref<Buffer>>(buffer);
}

template <IsTriviallyCopyable T>
const Buffer& BufIterator<T>::getStorage() const {
    switch (buffer.index()) {
    case 0:
        return std::get<ref<Buffer>>(buffer);
    case 1:
        return std::get<ref<const Buffer>>(buffer);
    default:
        throw std::out_of_range("");
    }
}

} // namespace gnev::gl