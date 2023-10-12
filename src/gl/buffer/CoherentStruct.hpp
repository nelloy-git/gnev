#pragma once

#include "gl/Buffer.hpp"

namespace gnev::gl::buffer {

template <IsTriviallyCopyable T>
class EXPORT CoherentStruct : public Buffer {
public:
    static constexpr GLbitfield STORAGE_FLAGS =
        GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
    static constexpr GLbitfield MAP_FLAGS =
        GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

    CoherentStruct(const T& initial_value = T{}, bool is_client_storage = false);
    CoherentStruct(const CoherentStruct& other) = delete;
    CoherentStruct(CoherentStruct&& other) = default;
    virtual ~CoherentStruct();

    T* operator->();
    const T* operator->() const;

    T& operator*();
    const T& operator*() const;

    T* get();
    const T* get() const;

private:
    T* mapped;
};

template <IsTriviallyCopyable T>
CoherentStruct<T>::CoherentStruct(const T& initial_value, bool is_client_storage)
    : Buffer() {
    Buffer::initStorage(sizeof(T),
                        &initial_value,
                        is_client_storage ? STORAGE_FLAGS | GL_CLIENT_STORAGE_BIT
                                          : STORAGE_FLAGS);
    mapped = static_cast<T*>(Buffer::mapRange(0, sizeof(T), MAP_FLAGS));
}

template <IsTriviallyCopyable T>
CoherentStruct<T>::~CoherentStruct() {}

template <IsTriviallyCopyable T>
T* CoherentStruct<T>::operator->() {
    return mapped;
}

template <IsTriviallyCopyable T>
const T* CoherentStruct<T>::operator->() const {
    return mapped;
}

template <IsTriviallyCopyable T>
T& CoherentStruct<T>::operator*() {
    return *mapped;
}

template <IsTriviallyCopyable T>
const T& CoherentStruct<T>::operator*() const {
    return *mapped;
}

template <IsTriviallyCopyable T>
T* CoherentStruct<T>::get() {
    return mapped;
}

template <IsTriviallyCopyable T>
const T* CoherentStruct<T>::get() const {
    return mapped;
}

} // namespace gnev::gl::buffer