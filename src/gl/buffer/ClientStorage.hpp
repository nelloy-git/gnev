#pragma once

#include "gl/buffer/ImmutableStorage.hpp"

namespace gnev::gl::buffer {

template <IsTriviallyCopyable T>
class EXPORT ClientStorage : public ImmutableStorage<T> {
public:
    ClientStorage(const Ctx& ctx, std::size_t capacity, const T& initial_value = T{});
    ClientStorage(const Ctx& ctx,
                  std::initializer_list<T> initial_data,
                  std::size_t capacity = initial_data.size());
    ClientStorage(const ClientStorage& other) = delete;
    ClientStorage(ClientStorage&& other) = default;
    virtual ~ClientStorage();

    T& operator[](std::size_t pos);
    const T& operator[](std::size_t pos) const;

    T* data();
    const T* data() const;

private:
    static constexpr GLbitfield MAP_FLAGS =
        GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

    T* mapped;
};

template <IsTriviallyCopyable T>
ClientStorage<T>::ClientStorage(const Ctx& ctx,
                                std::size_t capacity,
                                const T& initial_value)
    : ImmutableStorage<T>(ctx, GL_CLIENT_STORAGE_BIT, capacity, initial_value)
    , mapped(mapRange(0, capacity, MAP_FLAGS)) {}

template <IsTriviallyCopyable T>
ClientStorage<T>::ClientStorage(const Ctx& ctx,
                                std::initializer_list<T> initial_data,
                                std::size_t capacity)
    : ImmutableStorage<T>(ctx, GL_CLIENT_STORAGE_BIT, capacity, initial_data)
    , mapped(mapRange(0, capacity, MAP_FLAGS)) {}

template <IsTriviallyCopyable T>
ClientStorage<T>::~ClientStorage() {
    unmap();
}

template <IsTriviallyCopyable T>
T& ClientStorage<T>::operator[](std::size_t pos) {
    if (pos >= getCapacity()) {
        throw std::out_of_range("");
    }
    return *(&mapped[pos]);
}

template <IsTriviallyCopyable T>
const T& ClientStorage<T>::operator[](std::size_t pos) const {
    if (pos >= getCapacity()) {
        throw std::out_of_range("");
    }
    return *(&mapped[pos]);
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