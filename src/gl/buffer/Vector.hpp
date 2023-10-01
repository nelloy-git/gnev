#pragma once

#include "gl/buffer/ResizableStorage.hpp"

namespace gnev::gl::buffer {

template <IsTriviallyCopyable T>
class EXPORT Vector : public ResizableStorage<T> {
public:
    static constexpr float CAP_MULT = 2;

    Vector(const Ctx& ctx,
           GLenum usage = GL_DYNAMIC_COPY,
           std::size_t initial_size = 4,
           const T& initial_data = T{});
    Vector(const Vector& other) = delete;
    Vector(Vector&& other) = default;

    Vector(const Ctx& ctx, GLenum usage, std::size_t initial_size, const T* initial_data);
    virtual ~Vector();

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
    void reserve(std::size_t capacity);
    void shrinkToFit();
    void clear();

private:
    std::size_t size;
};

template <IsTriviallyCopyable T>
Vector<T>::Vector(const Ctx& ctx,
                  GLenum usage,
                  std::size_t initial_size,
                  const T& initial_data)
    : ResizableStorage<T>(ctx, usage, initial_size, initial_data)
    , size(initial_size) {}

template <IsTriviallyCopyable T>
Vector<T>::Vector(const Ctx& ctx,
                  GLenum usage,
                  std::size_t initial_size,
                  const T* initial_data)
    : ResizableStorage<T>(ctx, usage, initial_size, initial_data)
    , size(initial_size) {}

template <IsTriviallyCopyable T>
Vector<T>::~Vector() {}

template <IsTriviallyCopyable T>
void Vector<T>::setElement(std::size_t pos, const T& value) {
    setRange(pos, 1, &value);
}

template <IsTriviallyCopyable T>
void Vector<T>::copyElement(std::size_t src, std::size_t dst) {
    copyRange(src, dst, 1);
}

template <IsTriviallyCopyable T>
void Vector<T>::insertElement(std::size_t pos, const T& value) {
    insertRange(pos, 1, &value);
}

template <IsTriviallyCopyable T>
void Vector<T>::pushElementBack(const T& value) {
    insertRange(size, 1, &value);
}

template <IsTriviallyCopyable T>
void Vector<T>::eraseElement(std::size_t pos) {
    eraseRange(pos, 1);
}

template <IsTriviallyCopyable T>
T Vector<T>::getElement(std::size_t pos) const {
    if (pos >= size) {
        throw std::out_of_range("");
    }
    return ResizableStorage<T>::getElement(pos);
}

template <IsTriviallyCopyable T>
void Vector<T>::setRange(std::size_t first, std::size_t count, const T* src) {
    if (first + count > size) {
        throw std::out_of_range("");
    }
    ResizableStorage<T>::setRange(first, count, src);
}

template <IsTriviallyCopyable T>
void Vector<T>::copyRange(std::size_t src, std::size_t dst, std::size_t count) {
    if (src + count > size || dst + count > size) {
        throw std::out_of_range("");
    }
    ResizableStorage<T>::copyRange(src, dst, count);
}

template <IsTriviallyCopyable T>
void Vector<T>::insertRange(std::size_t first, std::size_t count, const T* src) {
    if (first > size) {
        throw std::out_of_range("");
    }

    while (size + count > ResizableStorage<T>::getCapacity()) {
        ResizableStorage<T>::setCapacity(CAP_MULT * ResizableStorage<T>::getCapacity());
    }

    if (first != size) {
        ResizableStorage<T>::copyRange(first, first + count, size - first);
    }
    ResizableStorage<T>::setRange(first, count, src);
    size += count;
}

template <IsTriviallyCopyable T>
void Vector<T>::pushRangeBack(std::size_t count, const T* src) {
    insertRange(size, count, src);
}

template <IsTriviallyCopyable T>
void Vector<T>::eraseRange(std::size_t first, std::size_t count) {
    if (first + count > size) {
        throw std::out_of_range("");
    }

    if (first + count != size) {
        ResizableStorage<T>::copyRange(first + count, first, size - (first + count));
    }
    size -= count;
}

template <IsTriviallyCopyable T>
std::vector<T> Vector<T>::getRange(std::size_t first, std::size_t count) const {
    if (first + count > size) {
        throw std::out_of_range("");
    }
    return ResizableStorage<T>::getRange(first, count);
}

template <IsTriviallyCopyable T>
std::size_t Vector<T>::getSize() const {
    return size;
}

template <IsTriviallyCopyable T>
void Vector<T>::reserve(std::size_t cap) {
    if (cap <= ResizableStorage<T>::getCapacity()) {
        return;
    }
    ResizableStorage<T>::setCapacity(cap);
}

template <IsTriviallyCopyable T>
void Vector<T>::shrinkToFit() {
    ResizableStorage<T>::setCapacity(size);
}

template <IsTriviallyCopyable T>
void Vector<T>::clear() {
    size = 0;
}

} // namespace gnev::gl::buffer