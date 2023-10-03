#pragma once

#include "gl/buffer/ImmutableStorage.hpp"

namespace gnev::gl::buffer {

template <IsTriviallyCopyable T>
class EXPORT ImmutableVector : public ImmutableStorage<T> {
public:
    static constexpr float CAP_MULT = 2;

    ImmutableVector(const Ctx& ctx,
                    GLbitfield storage_flags,
                    std::size_t capacity);
    ImmutableVector(const Ctx& ctx,
                    GLbitfield storage_flags,
                    std::size_t capacity,
                    std::initializer_list<T> initial_data);
    ImmutableVector(const ImmutableVector& other) = delete;
    ImmutableVector(ImmutableVector&& other) = default;
    virtual ~ImmutableVector();

    void setElement(std::size_t pos, const T& value);
    void copyElement(std::size_t src, std::size_t dst);
    T getElement(std::size_t pos) const;
    void insertElement(std::size_t pos, const T& value);
    void pushElementBack(const T& value);
    void eraseElement(std::size_t pos);

    void setRange(std::size_t first, std::size_t count, const T* src);
    void copyRange(std::size_t src, std::size_t dst, std::size_t count);
    std::vector<T> getRange(std::size_t first, std::size_t count) const;
    void insertRange(std::size_t first, std::size_t count, const T* src);
    void pushRangeBack(std::size_t count, const T* src);
    void eraseRange(std::size_t first, std::size_t count);

    std::size_t getSize() const;
    void clear();

private:
    std::size_t size;
};

template <IsTriviallyCopyable T>
ImmutableVector<T>::ImmutableVector(const Ctx& ctx,
                                    GLbitfield storage_flags,
                                    std::size_t capacity)
    : ImmutableStorage<T>(ctx, storage_flags, capacity, {})
    , size(0) {}

template <IsTriviallyCopyable T>
ImmutableVector<T>::ImmutableVector(const Ctx& ctx,
                                    GLbitfield storage_flags,
                                    std::size_t capacity,
                                    std::initializer_list<T> initial_data)
    : ImmutableStorage<T>(ctx, storage_flags, capacity, initial_data)
    , size(initial_data.size()) {}

template <IsTriviallyCopyable T>
ImmutableVector<T>::~ImmutableVector() {}

template <IsTriviallyCopyable T>
void ImmutableVector<T>::setElement(std::size_t pos, const T& value) {
    setRange(pos, 1, &value);
}

template <IsTriviallyCopyable T>
void ImmutableVector<T>::copyElement(std::size_t src, std::size_t dst) {
    copyRange(src, dst, 1);
}

template <IsTriviallyCopyable T>
void ImmutableVector<T>::insertElement(std::size_t pos, const T& value) {
    insertRange(pos, 1, &value);
}

template <IsTriviallyCopyable T>
void ImmutableVector<T>::pushElementBack(const T& value) {
    pushRangeBack(1, value);
}

template <IsTriviallyCopyable T>
void ImmutableVector<T>::eraseElement(std::size_t pos) {
    eraseRange(pos, 1);
}

template <IsTriviallyCopyable T>
T ImmutableVector<T>::getElement(std::size_t pos) const {
    if (pos >= size) {
        throw std::out_of_range("");
    }
    return ImmutableStorage<T>::getElement(pos);
}

template <IsTriviallyCopyable T>
void ImmutableVector<T>::setRange(std::size_t first, std::size_t count, const T* src) {
    if (first + count > size) {
        throw std::out_of_range("");
    }
    ImmutableStorage<T>::setRange(first, count, src);
}

template <IsTriviallyCopyable T>
void ImmutableVector<T>::copyRange(std::size_t src, std::size_t dst, std::size_t count) {
    if (src + count > size || dst + count > size) {
        throw std::out_of_range("");
    }
    ImmutableStorage<T>::copyRange(src, dst, count);
}

template <IsTriviallyCopyable T>
void ImmutableVector<T>::insertRange(std::size_t first, std::size_t count, const T* src) {
    if (first > size) {
        throw std::out_of_range("");
    }

    if (first != size) {
        ImmutableStorage<T>::copyRange(first, first + count, size - first);
    }
    ImmutableStorage<T>::setRange(first, count, src);
    size += count;
}

template <IsTriviallyCopyable T>
void ImmutableVector<T>::pushRangeBack(std::size_t count, const T* src) {
    insertRange(size, count, src);
}

template <IsTriviallyCopyable T>
void ImmutableVector<T>::eraseRange(std::size_t first, std::size_t count) {
    if (first + count > size) {
        throw std::out_of_range("");
    }

    if (first + count != size) {
        ImmutableStorage<T>::copyRange(first + count, first, size - (first + count));
    }
    size -= count;
}

template <IsTriviallyCopyable T>
std::vector<T> ImmutableVector<T>::getRange(std::size_t first, std::size_t count) const {
    if (first + count > size) {
        throw std::out_of_range("");
    }
    return ImmutableStorage<T>::getRange(first, count);
}

template <IsTriviallyCopyable T>
std::size_t ImmutableVector<T>::getSize() const {
    return size;
}

template <IsTriviallyCopyable T>
void ImmutableVector<T>::clear() {
    size = 0;
}

} // namespace gnev::gl::buffer