#pragma once

#include "gl/buffer/ImmutableStorage.hpp"

namespace gnev::gl::buffer {

template <IsTriviallyCopyable T>
class EXPORT ClientStorage : public ImmutableStorage<T> {
public:
    static constexpr GLbitfield STORAGE_FLAGS = GL_CLIENT_STORAGE_BIT;
    static constexpr GLbitfield MAP_FLAGS =
        GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

    ClientStorage(const Ctx& ctx, std::size_t capacity, const T& initial_value = T{});
    ClientStorage(const Ctx& ctx,
                  std::size_t capacity,
                  std::initializer_list<T> initial_data);
    ClientStorage(const ClientStorage& other) = delete;
    ClientStorage(ClientStorage&& other) = default;
    virtual ~ClientStorage();

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

private:
    T* mapped;
};

template <IsTriviallyCopyable T>
ClientStorage<T>::ClientStorage(const Ctx& ctx,
                                std::size_t capacity,
                                const T& initial_value)
    : ImmutableStorage<T>(ctx, STORAGE_FLAGS, capacity, initial_value)
    , mapped(ImmutableStorage<T>::mapRange(0, capacity, MAP_FLAGS)) {}

template <IsTriviallyCopyable T>
ClientStorage<T>::ClientStorage(const Ctx& ctx,
                                std::size_t capacity,
                                std::initializer_list<T> initial_data)
    : ImmutableStorage<T>(ctx, STORAGE_FLAGS, capacity, initial_data)
    , mapped(ImmutableStorage<T>::mapRange(0, capacity, MAP_FLAGS)) {}

template <IsTriviallyCopyable T>
ClientStorage<T>::~ClientStorage() {
    ImmutableStorage<T>::unmap();
}

template <IsTriviallyCopyable T>
T& ClientStorage<T>::at(std::size_t pos) {
    if (pos >= ImmutableStorage<T>::getCapacity()) {
        throw std::out_of_range("");
    }
    return mapped[pos];
}

template <IsTriviallyCopyable T>
const T& ClientStorage<T>::at(std::size_t pos) const {
    if (pos >= ImmutableStorage<T>::getCapacity()) {
        throw std::out_of_range("");
    }
    return mapped[pos];
}

template <IsTriviallyCopyable T>
T& ClientStorage<T>::operator[](std::size_t pos) {
    return mapped[pos];
}

template <IsTriviallyCopyable T>
const T& ClientStorage<T>::operator[](std::size_t pos) const {
    return mapped[pos];
}

template <IsTriviallyCopyable T>
T& ClientStorage<T>::front() {
    return mapped[0];
}

template <IsTriviallyCopyable T>
const T& ClientStorage<T>::front() const {
    return mapped[0];
}

template <IsTriviallyCopyable T>
T& ClientStorage<T>::back() {
    return mapped[ImmutableStorage<T>::getCapacity() - 1];
}

template <IsTriviallyCopyable T>
const T& ClientStorage<T>::back() const {
    return mapped[ImmutableStorage<T>::getCapacity() - 1];
}

template <IsTriviallyCopyable T>
T* ClientStorage<T>::begin() {
    return mapped;
}

template <IsTriviallyCopyable T>
const T* ClientStorage<T>::begin() const {
    return mapped;
}

template <IsTriviallyCopyable T>
T* ClientStorage<T>::end() {
    return mapped + ImmutableStorage<T>::getCapacity();
}

template <IsTriviallyCopyable T>
const T* ClientStorage<T>::end() const {
    return mapped + ImmutableStorage<T>::getCapacity();
}

template <IsTriviallyCopyable T>
T* ClientStorage<T>::data() {
    return mapped;
}

template <IsTriviallyCopyable T>
const T* ClientStorage<T>::data() const {
    return mapped;
}

} // namespace gnev::gl::buffer