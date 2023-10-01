#pragma once

#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include "gl/Buffer.hpp"

namespace gnev::gl::buffer {

template <IsTriviallyCopyable T>
class EXPORT ResizableStorage : public Buffer {
public:
    using Element = T;

    ResizableStorage(const Ctx& ctx,
                     GLenum usage,
                     std::size_t initial_capacity,
                     const T& initial_data = T{});
    ResizableStorage(const Ctx& ctx,
                     GLenum usage,
                     std::size_t initial_capacity,
                     std::initializer_list<T> initial_data);
    ResizableStorage(const ResizableStorage& other) = delete;
    ResizableStorage(ResizableStorage&& other) = default;
    virtual ~ResizableStorage();

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
ResizableStorage<T>::ResizableStorage(const Ctx& ctx,
                                      GLenum usage,
                                      std::size_t initial_capacity,
                                      const T& initial_data)
    : Buffer(ctx)
    , usage(usage)
    , buffer_size(std::max(initial_capacity * sizeof(T), sizeof(T))) {
    std::vector<T> tmp(initial_capacity, initial_data);
    initData(buffer_size, tmp.data(), usage);
}

template <IsTriviallyCopyable T>
ResizableStorage<T>::ResizableStorage(const Ctx& ctx,
                                      GLenum usage,
                                      std::size_t initial_capacity,
                                      std::initializer_list<T> initial_data)
    : Buffer(ctx)
    , usage(usage)
    , buffer_size(std::max(initial_capacity * sizeof(T), sizeof(T))) {
    std::size_t initializer_size = initial_data.size() * sizeof(T);
    initData(buffer_size, nullptr, usage);
    setSubData(0, initializer_size, initial_data.begin());
}

template <IsTriviallyCopyable T>
ResizableStorage<T>::~ResizableStorage() {}

template <IsTriviallyCopyable T>
void ResizableStorage<T>::setElement(std::size_t pos, const T& value) {
    if (pos >= getCapacity()) {
        throw std::out_of_range("");
    }
    setSubData(pos * sizeof(T), sizeof(T), &value);
}

template <IsTriviallyCopyable T>
void ResizableStorage<T>::copyElement(std::size_t src, std::size_t dst) {
    if (src >= getCapacity() || dst >= getCapacity()) {
        throw std::out_of_range("");
    }
    if (src != dst) {
        copyTo(*this, src * sizeof(T), dst * sizeof(T), 1);
    }
}

template <IsTriviallyCopyable T>
T ResizableStorage<T>::getElement(std::size_t pos) const {
    if (pos >= getCapacity()) {
        throw std::out_of_range("");
    }
    T result;
    getSubData(pos * sizeof(T), sizeof(T), &result);
    return result;
}

template <IsTriviallyCopyable T>
void ResizableStorage<T>::setRange(std::size_t first, std::size_t count, const T* src) {
    if (first + count > getCapacity()) {
        throw std::out_of_range("");
    }
    setSubData(first * sizeof(T), count * sizeof(T), src);
}

template <IsTriviallyCopyable T>
void ResizableStorage<T>::copyRange(std::size_t src, std::size_t dst, std::size_t count) {
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
std::vector<T> ResizableStorage<T>::getRange(std::size_t first, std::size_t count) const {
    if (first + count > getCapacity()) {
        throw std::out_of_range("");
    }
    std::vector<T> result(count, T{});
    getSubData(first * sizeof(T), count * sizeof(T), result.data());
    return result;
}

template <IsTriviallyCopyable T>
void ResizableStorage<T>::setCapacity(std::size_t cap) {
    std::size_t tmp_buffer_size = std::min<GLsizeiptr>(buffer_size, cap * sizeof(T));

    Buffer tmp_buffer(ctx());
    tmp_buffer.initStorage(tmp_buffer_size, nullptr, 0);
    copyTo(tmp_buffer, 0, 0, tmp_buffer_size);

    initData(cap * sizeof(T), nullptr, usage);
    tmp_buffer.copyTo(*this, 0, 0, tmp_buffer_size);

    if (getCapacity() < cap) {
        std::size_t elements_to_fill = cap - getCapacity();
        std::vector<T> tmp(elements_to_fill, T{});
        setSubData(buffer_size, elements_to_fill * sizeof(T), tmp.data());
    }

    buffer_size = cap * sizeof(T);
}

template <IsTriviallyCopyable T>
std::size_t ResizableStorage<T>::getCapacity() const {
    return buffer_size / sizeof(T);
}

template <IsTriviallyCopyable T>
void ResizableStorage<T>::setUsage(GLenum new_usage) {
    usage = new_usage;
    setCapacity(getCapacity());
}

template <IsTriviallyCopyable T>
GLenum ResizableStorage<T>::getUsage() const {
    return usage;
}

} // namespace gnev::gl::buffer