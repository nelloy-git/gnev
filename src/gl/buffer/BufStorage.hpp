#pragma once

#include "gl/Buffer.hpp"
#include "gl/buffer/BufStorageIterator.hpp"

namespace gnev::gl {

template <IsTriviallyCopyable T>
class EXPORT BufStorage : public Buffer {
public:
    BufStorage(GLbitfield storage_flags,
               GLuint capacity,
               std::initializer_list<T> initial_list = {},
               const T& initial_data = T{});
    BufStorage(const BufStorage& other) = delete;
    BufStorage(BufStorage&& other) = default;
    virtual ~BufStorage();

    BufStorageIterator<T> operator[](GLuint index);
    const BufStorageIterator<T> operator[](GLuint index) const;

    BufStorageIterator<T> at(GLuint index);
    const BufStorageIterator<T> at(GLuint index) const;

    T* map(GLenum access);
    T* mapRange(GLuint first, GLuint count, GLbitfield access);
    void flushRange(GLuint offset, GLuint count);

    GLuint getCapacity() const;
    GLbitfield getStorageFlags() const;

private:
    GLuint capacity;
};

template <IsTriviallyCopyable T>
BufStorage<T>::BufStorage(GLbitfield storage_flags,
                          GLuint capacity,
                          std::initializer_list<T> initial_list,
                          const T& initial_data)
    : Buffer()
    , capacity(capacity) {
    if (capacity == 0 || initial_list.size() > capacity) {
        throw std::out_of_range("");
    }

    std::vector<T> initial(capacity, initial_data);
    std::copy(initial_list.begin(), initial_list.end(), initial.begin());
    initStorage(capacity * sizeof(T), initial.data(), storage_flags);
}

template <IsTriviallyCopyable T>
BufStorage<T>::~BufStorage() {}

template <IsTriviallyCopyable T>
BufStorageIterator<T> BufStorage<T>::operator[](GLuint index) {
    return at(index);
}

template <IsTriviallyCopyable T>
const BufStorageIterator<T> BufStorage<T>::operator[](GLuint index) const {
    return at(index);
}

template <IsTriviallyCopyable T>
BufStorageIterator<T> BufStorage<T>::at(GLuint index) {
    if (index >= capacity) {
        throw std::out_of_range("");
    }
    return BufStorageIterator<T>(*this, index);
}

template <IsTriviallyCopyable T>
const BufStorageIterator<T> BufStorage<T>::at(GLuint index) const {
    if (index >= capacity) {
        throw std::out_of_range("");
    }
    return BufStorageIterator<T>(*this, index);
}

template <IsTriviallyCopyable T>
T* BufStorage<T>::map(GLenum access) {
    return reinterpret_cast<T*>(Buffer::map(access));
}

template <IsTriviallyCopyable T>
T* BufStorage<T>::mapRange(GLuint first, GLuint count, GLbitfield access) {
    return reinterpret_cast<T*>(Buffer::mapRange(first * sizeof(T),
                                                 count * sizeof(T),
                                                 access));
}

template <IsTriviallyCopyable T>
void BufStorage<T>::flushRange(GLuint first, GLuint count) {
    Buffer::flushRange(first * sizeof(T), count * sizeof(T));
}

template <IsTriviallyCopyable T>
GLuint BufStorage<T>::getCapacity() const {
    return capacity;
}

template <IsTriviallyCopyable T>
GLuint BufStorage<T>::getStorageFlags() const {
    GLbitfield storage_flags;
    getParameteriv(GL_BUFFER_STORAGE_FLAGS, reinterpret_cast<GLint*>(&storage_flags));
    return storage_flags;
}

} // namespace gnev::gl