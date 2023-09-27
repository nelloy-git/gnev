#pragma once

#include "gl/BufferResizable.hpp"

namespace gnev::gl {

template <IsTriviallyCopyable T>
class EXPORT BufferVector : public BufferResizable<T> {
public:
    using type = T;
    static constexpr GLsizeiptr base_cap = 4;

    BufferVector(const GladCtx& ctx,
                 GLenum usage = GL_DYNAMIC_COPY,
                 std::size_t initial_size = 4,
                 const T& initial_data = T{});
    virtual ~BufferVector();

    void setElement(std::size_t pos, const T& value);
    void insertElement(std::size_t pos, const T& value);
    void pushElementBack(const T& value);
    void eraseElement(std::size_t pos);
    T getElement(std::size_t pos) const;

    void setRange(std::size_t first, std::size_t count, const T* src);
    void insertRange(std::size_t first, std::size_t count, const T* src);
    void pushRangeBack(std::size_t count, const T* src);
    void eraseRange(std::size_t first, std::size_t count);
    std::vector<T> getRange(std::size_t first, std::size_t count) const;

    std::size_t getSize() const;
    void reserve(std::size_t capacity);
    void shrinkToFit();
    void clear();

private:
    std::size_t size;
};

template <IsTriviallyCopyable T>
BufferVector<T>::BufferVector(const GladCtx& ctx,
                              GLenum usage,
                              std::size_t initial_capacity,
                              const T& initial_data)
    : BufferResizable<T>(ctx, usage, initial_capacity, initial_data)
    , size(0) {}

template <IsTriviallyCopyable T>
BufferVector<T>::~BufferVector() {}

template <IsTriviallyCopyable T>
void BufferVector<T>::setElement(std::size_t pos, const T& value) {
    setRange(pos, 1, &value);
}

template <IsTriviallyCopyable T>
void BufferVector<T>::insertElement(std::size_t pos, const T& value) {
    insertRange(pos, 1, &value);
}

template <IsTriviallyCopyable T>
void BufferVector<T>::pushElementBack(const T& value) {
    insertRange(size, 1, &value);
}

template <IsTriviallyCopyable T>
void BufferVector<T>::eraseElement(std::size_t pos) {
    eraseRange(pos, 1);
}

template <IsTriviallyCopyable T>
T BufferVector<T>::getElement(std::size_t pos) const {
    if (pos >= size) {
        throw std::out_of_range("");
    }
    return BufferResizable<T>::getElement(pos);
}

template <IsTriviallyCopyable T>
void BufferVector<T>::setRange(std::size_t first, std::size_t count, const T* src) {
    if (first + count > size) {
        throw std::out_of_range("");
    }
    BufferResizable<T>::setRange(first, count, src);
}

template <IsTriviallyCopyable T>
void BufferVector<T>::insertRange(std::size_t first, std::size_t count, const T* src) {
    if (first > size) {
        throw std::out_of_range("");
    }

    while (size + count > BufferResizable<T>::getCapacity()) {
        BufferResizable<T>::setCapacity(2 * BufferResizable<T>::getCapacity());
    }

    if (first != size) {
        BufferResizable<T>::copyRange(first, count, size - first);
    }
    BufferResizable<T>::setRange(first, count, src);
    size += count;
}

template <IsTriviallyCopyable T>
void BufferVector<T>::pushRangeBack(std::size_t count, const T* src) {
    insertRange(size, count, src);
}

template <IsTriviallyCopyable T>
void BufferVector<T>::eraseRange(std::size_t first, std::size_t count) {
    if (first + count >= size) {
        throw std::out_of_range("");
    }

    if (first + count != size) {
        BufferResizable<T>::copyRange(first + count, first, size - first - count);
    }
    size -= count;
}

template <IsTriviallyCopyable T>
std::vector<T> BufferVector<T>::getRange(std::size_t first, std::size_t count) const {
    if (first + count > size) {
        throw std::out_of_range("");
    }
    return BufferResizable<T>::getRange(first, count);
}

template <IsTriviallyCopyable T>
std::size_t BufferVector<T>::getSize() const {
    return size;
}

template <IsTriviallyCopyable T>
void BufferVector<T>::reserve(std::size_t cap) {
    if (cap <= BufferResizable<T>::getCapacity()) {
        return;
    }
    BufferResizable<T>::setCapacity(cap);
}

template <IsTriviallyCopyable T>
void BufferVector<T>::shrinkToFit() {
    BufferResizable<T>::setCapacity(size);
}

template <IsTriviallyCopyable T>
void BufferVector<T>::clear() {
    size = 0;
}

} // namespace gnev::gl