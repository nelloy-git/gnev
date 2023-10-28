#pragma once

#include <initializer_list>
#include <stdexcept>

#include "gl/Buffer.hpp"

namespace gnev::gl::buffer {

template <IsTriviallyCopyable T>
class EXPORT CoherentStorage : public Buffer {
public:
    static constexpr GLbitfield STORAGE_FLAGS =
        GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
    static constexpr GLbitfield MAP_FLAGS =
        GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

    CoherentStorage(std::size_t capacity,
                    const T& clean_up = T{},
                    bool is_client_storage = false);
    CoherentStorage(std::initializer_list<T> initial_data,
                    bool is_client_storage = false);
    CoherentStorage(const CoherentStorage& other) = delete;
    CoherentStorage(CoherentStorage&& other) = default;
    virtual ~CoherentStorage();

    T& at(std::size_t pos);
    const T& at(std::size_t pos) const;

    T& operator[](std::size_t pos);
    const T& operator[](std::size_t pos) const;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    T* begin();
    const T* begin() const;
    T* end();
    const T* end() const;

    T* data();
    const T* data() const;

    std::size_t getCapacity() const;

private:
    const std::size_t capacity;
    T* mapped;
};

template <IsTriviallyCopyable T>
CoherentStorage<T>::CoherentStorage(std::size_t capacity,
                                    const T& clean_up,
                                    bool is_client_storage)
    : Buffer()
    , capacity(capacity) {
    if (capacity == 0) {
        throw std::out_of_range("");
    }

    Buffer::initStorage(capacity * sizeof(T),
                        nullptr,
                        is_client_storage ? STORAGE_FLAGS | GL_CLIENT_STORAGE_BIT
                                          : STORAGE_FLAGS);
    mapped = static_cast<T*>(Buffer::mapRange(0, capacity * sizeof(T), MAP_FLAGS));
    std::fill_n(mapped, capacity, clean_up);
}

template <IsTriviallyCopyable T>
CoherentStorage<T>::CoherentStorage(std::initializer_list<T> initial_data,
                                    bool is_client_storage)
    : Buffer()
    , capacity(initial_data.size()) {

    Buffer::initStorage(capacity * sizeof(T),
                        initial_data.begin(),
                        is_client_storage ? STORAGE_FLAGS | GL_CLIENT_STORAGE_BIT
                                          : STORAGE_FLAGS);
    mapped = static_cast<T*>(Buffer::mapRange(0, capacity * sizeof(T), MAP_FLAGS));
}

template <IsTriviallyCopyable T>
CoherentStorage<T>::~CoherentStorage() {}

template <IsTriviallyCopyable T>
T& CoherentStorage<T>::at(std::size_t pos) {
    if (pos >= capacity) {
        throw std::out_of_range("");
    }
    return mapped[pos];
}

template <IsTriviallyCopyable T>
const T& CoherentStorage<T>::at(std::size_t pos) const {
    if (pos >= capacity) {
        throw std::out_of_range("");
    }
    return mapped[pos];
}

template <IsTriviallyCopyable T>
T& CoherentStorage<T>::operator[](std::size_t pos) {
    return mapped[pos];
}

template <IsTriviallyCopyable T>
const T& CoherentStorage<T>::operator[](std::size_t pos) const {
    return mapped[pos];
}

template <IsTriviallyCopyable T>
T& CoherentStorage<T>::front() {
    return mapped[0];
}

template <IsTriviallyCopyable T>
const T& CoherentStorage<T>::front() const {
    return mapped[0];
}

template <IsTriviallyCopyable T>
T& CoherentStorage<T>::back() {
    return mapped[capacity - 1];
}

template <IsTriviallyCopyable T>
const T& CoherentStorage<T>::back() const {
    return mapped[capacity - 1];
}

template <IsTriviallyCopyable T>
T* CoherentStorage<T>::begin() {
    return mapped;
}

template <IsTriviallyCopyable T>
const T* CoherentStorage<T>::begin() const {
    return mapped;
}

template <IsTriviallyCopyable T>
T* CoherentStorage<T>::end() {
    return mapped + capacity - 1;
}

template <IsTriviallyCopyable T>
const T* CoherentStorage<T>::end() const {
    return mapped + capacity - 1;
}

template <IsTriviallyCopyable T>
T* CoherentStorage<T>::data() {
    return mapped;
}

template <IsTriviallyCopyable T>
const T* CoherentStorage<T>::data() const {
    return mapped;
}

template <IsTriviallyCopyable T>
std::size_t CoherentStorage<T>::getCapacity() const {
    return capacity;
}

} // namespace gnev::gl::buffer