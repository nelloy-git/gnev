#pragma once

#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include "gl/Buffer.hpp"

namespace gnev::gl::buffer {

template <IsTriviallyCopyable T>
class EXPORT Resizable : public Buffer {
public:
    using Element = T;

    Resizable(const Ctx& ctx,
              GLenum usage,
              std::size_t initial_capacity,
              const T& initial_data = T{});
    Resizable(const Ctx& ctx,
              GLenum usage,
              std::size_t initial_capacity,
              std::initializer_list<T> initial_data);
    Resizable(const Resizable& other) = delete;
    Resizable(Resizable&& other) = default;
    virtual ~Resizable();

    void setElement(std::size_t pos, const T& value);
    void copyElement(std::size_t src, std::size_t dst);
    T getElement(std::size_t pos) const;

    void setRange(std::size_t first, std::size_t count, const T* src);
    void copyRange(std::size_t src, std::size_t dst, std::size_t count);
    std::vector<T> getRange(std::size_t first, std::size_t count) const;

    void setCapacity(std::size_t capacity);
    std::size_t getCapacity() const;

    void setUsage(GLenum usage);
    GLenum getUsage() const;

private:
    GLenum usage;
    GLsizeiptr buffer_size;
};

template <IsTriviallyCopyable T>
Resizable<T>::Resizable(const Ctx& ctx,
                        GLenum usage,
                        std::size_t initial_capacity,
                        const T& initial_data)
    : Buffer(ctx)
    , usage(usage)
    , buffer_size(std::max(initial_capacity * sizeof(T), sizeof(T))) {
    std::vector<T> tmp(initial_capacity, initial_data);
    glBufferData(buffer_size, tmp.data(), usage);
}

template <IsTriviallyCopyable T>
Resizable<T>::Resizable(const Ctx& ctx,
                        GLenum usage,
                        std::size_t initial_capacity,
                        std::initializer_list<T> initial_data)
    : Buffer(ctx)
    , usage(usage)
    , buffer_size(std::max(initial_capacity * sizeof(T), sizeof(T))) {
    std::size_t initializer_size = initial_data.size() * sizeof(T);
    glBufferData(buffer_size, nullptr, usage);
    glBufferSubData(0, initializer_size, initial_data.begin());
}

template <IsTriviallyCopyable T>
Resizable<T>::~Resizable() {}

template <IsTriviallyCopyable T>
void Resizable<T>::setElement(std::size_t pos, const T& value) {
    if (pos >= getCapacity()) {
        throw std::out_of_range("");
    }
    glBufferSubData(pos * sizeof(T), sizeof(T), &value);
}

template <IsTriviallyCopyable T>
void Resizable<T>::copyElement(std::size_t src, std::size_t dst) {
    if (src >= getCapacity() || dst >= getCapacity()) {
        throw std::out_of_range("");
    }
    if (src != dst) {
        glCopyBufferSubData(handle(), src * sizeof(T), dst * sizeof(T), 1);
    }
}

template <IsTriviallyCopyable T>
T Resizable<T>::getElement(std::size_t pos) const {
    if (pos >= getCapacity()) {
        throw std::out_of_range("");
    }
    T result;
    glGetBufferSubData(pos * sizeof(T), sizeof(T), &result);
    return result;
}

template <IsTriviallyCopyable T>
void Resizable<T>::setRange(std::size_t first, std::size_t count, const T* src) {
    if (first + count > getCapacity()) {
        throw std::out_of_range("");
    }
    glBufferSubData(first * sizeof(T), count * sizeof(T), src);
}

template <IsTriviallyCopyable T>
void Resizable<T>::copyRange(std::size_t src, std::size_t dst, std::size_t count) {
    if (src + count > getCapacity() || dst + count > getCapacity()) {
        throw std::out_of_range("");
    }

    if (src >= dst + count || src + count <= dst) {
        glCopyBufferSubData(handle(),
                            src * sizeof(T),
                            dst * sizeof(T),
                            count * sizeof(T));
    } else {
        Buffer tmp_buffer(ctx());
        tmp_buffer.glBufferStorage(count * sizeof(T), nullptr, 0);
        glCopyBufferSubData(tmp_buffer.handle(), src * sizeof(T), 0, count * sizeof(T));
        tmp_buffer.glCopyBufferSubData(handle(), 0, dst * sizeof(T), count * sizeof(T));
    }
}

template <IsTriviallyCopyable T>
std::vector<T> Resizable<T>::getRange(std::size_t first, std::size_t count) const {
    if (first + count > getCapacity()) {
        throw std::out_of_range("");
    }
    std::vector<T> result(count, T{});
    glGetBufferSubData(first * sizeof(T), count * sizeof(T), result.data());
    return result;
}

template <IsTriviallyCopyable T>
void Resizable<T>::setCapacity(std::size_t cap) {
    std::size_t tmp_buffer_size = std::min<GLsizeiptr>(buffer_size, cap * sizeof(T));

    Buffer tmp_buffer(ctx());
    tmp_buffer.glBufferStorage(tmp_buffer_size, nullptr, 0);
    glCopyBufferSubData(tmp_buffer.handle(), 0, 0, tmp_buffer_size);

    glBufferData(cap * sizeof(T), nullptr, usage);
    tmp_buffer.glCopyBufferSubData(handle(), 0, 0, tmp_buffer_size);

    if (getCapacity() < cap) {
        std::size_t elements_to_fill = cap - getCapacity();
        std::vector<T> tmp(elements_to_fill, T{});
        glBufferSubData(buffer_size, elements_to_fill * sizeof(T), tmp.data());
    }

    buffer_size = cap * sizeof(T);
}

template <IsTriviallyCopyable T>
std::size_t Resizable<T>::getCapacity() const {
    return buffer_size / sizeof(T);
}

template <IsTriviallyCopyable T>
void Resizable<T>::setUsage(GLenum new_usage) {
    usage = new_usage;
    setCapacity(getCapacity());
}

template <IsTriviallyCopyable T>
GLenum Resizable<T>::getUsage() const {
    return usage;
}

} // namespace gnev::gl::buffer