#pragma once

#include <initializer_list>
#include <stdexcept>

#include "gl/Buffer.hpp"

namespace gnev::gl::buffer {

template <IsTriviallyCopyable T>
class EXPORT ImmutableStorage : public Buffer {
public:
    using Element = T;

    ImmutableStorage(const Ctx& ctx,
                     GLbitfield storage_flags,
                     std::size_t capacity,
                     const T& initial_value = T{});
    ImmutableStorage(const Ctx& ctx,
                     GLbitfield storage_flags,
                     std::size_t capacity,
                     std::initializer_list<T> initial_data);
    ImmutableStorage(const ImmutableStorage& other) = delete;
    ImmutableStorage(ImmutableStorage&& other) = default;
    virtual ~ImmutableStorage();

    void setElement(std::size_t pos, const T& value, bool use_tmp_buffer = false);
    void copyElement(std::size_t src, std::size_t dst);
    T getElement(std::size_t pos) const;

    void setRange(std::size_t first,
                  std::size_t count,
                  const T* src,
                  bool use_tmp_buffer = false);
    void copyRange(std::size_t src, std::size_t dst, std::size_t count);
    void fillRange(std::size_t first,
                   std::size_t count,
                   const T& value,
                   bool use_tmp_buffer = false);
    std::vector<T> getRange(std::size_t first, std::size_t count) const;

    T* map(GLenum access);
    T* mapRange(std::size_t first, std::size_t count, GLbitfield access);
    void flushRange(std::size_t offset, std::size_t count);

    std::size_t getCapacity() const;
    GLbitfield getStorageFlags() const;

private:
    const std::size_t capacity;
};

template <IsTriviallyCopyable T>
ImmutableStorage<T>::ImmutableStorage(const Ctx& ctx,
                                      GLbitfield storage_flags,
                                      std::size_t capacity,
                                      const T& initial_value)
    : Buffer(ctx)
    , capacity(capacity) {
    if (capacity == 0) {
        throw std::out_of_range("");
    }

    initStorage(capacity * sizeof(T), nullptr, storage_flags);
    fillRange(0, capacity, initial_value, !(storage_flags & GL_DYNAMIC_STORAGE_BIT));
}

template <IsTriviallyCopyable T>
ImmutableStorage<T>::ImmutableStorage(const Ctx& ctx,
                                      GLbitfield storage_flags,
                                      std::size_t capacity,
                                      std::initializer_list<T> initial_data)
    : Buffer(ctx)
    , capacity(capacity) {
    if (initial_data.size() == capacity) {
        initStorage(capacity * sizeof(T), initial_data.begin(), storage_flags);
    } else {
        initStorage(capacity * sizeof(T), nullptr, storage_flags);
        setRange(0,
                 initial_data.size(),
                 initial_data.begin(),
                 !(storage_flags & GL_DYNAMIC_STORAGE_BIT));
    }
}

template <IsTriviallyCopyable T>
ImmutableStorage<T>::~ImmutableStorage() {}

template <IsTriviallyCopyable T>
void ImmutableStorage<T>::setElement(std::size_t pos,
                                     const T& value,
                                     bool use_tmp_buffer) {
    setRange(pos, 1, &value, use_tmp_buffer);
}

template <IsTriviallyCopyable T>
void ImmutableStorage<T>::copyElement(std::size_t src, std::size_t dst) {
    copyRange(src, dst, 1);
}

template <IsTriviallyCopyable T>
T ImmutableStorage<T>::getElement(std::size_t pos) const {
    if (pos >= getCapacity()) {
        throw std::out_of_range("");
    }

    T result;
    getSubData(pos * sizeof(T), sizeof(T), &result);
    return result;
}

template <IsTriviallyCopyable T>
void ImmutableStorage<T>::setRange(std::size_t first,
                                   std::size_t count,
                                   const T* src,
                                   bool use_tmp_buffer) {
    if (first + count > getCapacity()) {
        throw std::out_of_range("");
    }

    if (use_tmp_buffer) {
        Buffer tmp_buffer(ctx());
        tmp_buffer.initStorage(count * sizeof(T), src, 0);
        tmp_buffer.copyTo(*this, 0, first * sizeof(T), count * sizeof(T));
    } else {
        setSubData(first * sizeof(T), count * sizeof(T), src);
    }
}

template <IsTriviallyCopyable T>
void ImmutableStorage<T>::copyRange(std::size_t src, std::size_t dst, std::size_t count) {
    if (src + count > getCapacity() || dst + count > getCapacity()) {
        throw std::out_of_range("");
    }

    if (src >= dst + count || src + count <= dst) {
        copyTo(*this, src * sizeof(T), dst * sizeof(T), count * sizeof(T));
    } else {
        Buffer tmp_buffer(ctx());
        tmp_buffer.initStorage(count * sizeof(T), nullptr, 0);
        copyTo(tmp_buffer, src * sizeof(T), 0, count * sizeof(T));
        tmp_buffer.copyTo(*this, 0, dst * sizeof(T), count * sizeof(T));
    }
}

template <IsTriviallyCopyable T>
void ImmutableStorage<T>::fillRange(std::size_t first,
                                    std::size_t count,
                                    const T& value,
                                    bool use_tmp_buffer) {
    std::vector<T> initial_data(count, value);

    if (use_tmp_buffer) {
        Buffer tmp_buffer(ctx());
        tmp_buffer.initStorage(count * sizeof(T), initial_data.data(), 0);
        tmp_buffer.copyTo(*this, 0, first * sizeof(T), count * sizeof(T));
    } else {
        setSubData(first * sizeof(T), count * sizeof(T), initial_data.data());
    }
}

template <IsTriviallyCopyable T>
std::vector<T> ImmutableStorage<T>::getRange(std::size_t first, std::size_t count) const {
    if (first + count > getCapacity()) {
        throw std::out_of_range("");
    }

    std::vector<T> result(count);
    getSubData(first * sizeof(T), count * sizeof(T), result.data());
    return result;
}

template <IsTriviallyCopyable T>
std::size_t ImmutableStorage<T>::getCapacity() const {
    return capacity;
}

template <IsTriviallyCopyable T>
GLbitfield ImmutableStorage<T>::getStorageFlags() const {
    GLbitfield storage_flags;
    getParameteriv(GL_BUFFER_STORAGE_FLAGS, reinterpret_cast<GLint*>(&storage_flags));
    return storage_flags;
}

template <IsTriviallyCopyable T>
T* ImmutableStorage<T>::map(GLenum access) {
    return reinterpret_cast<T*>(Buffer::map(access));
}

template <IsTriviallyCopyable T>
T* ImmutableStorage<T>::mapRange(std::size_t first,
                                 std::size_t count,
                                 GLbitfield access) {
    return reinterpret_cast<T*>(Buffer::mapRange(first * sizeof(T),
                                                 count * sizeof(T),
                                                 access));
}

template <IsTriviallyCopyable T>
void ImmutableStorage<T>::flushRange(std::size_t first, std::size_t count) {
    Buffer::flushRange(first * sizeof(T), count * sizeof(T));
}

} // namespace gnev::gl::buffer