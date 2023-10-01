#pragma once

#include <algorithm>
#include <array>
#include <stdexcept>

#include "gl/Buffer.hpp"

namespace gnev::gl::buffer {

template <IsTriviallyCopyable T>
class EXPORT Array : public Buffer {
public:
    using Element = T;

    Array(const Ctx& ctx,
          GLbitfield storage_flags,
          GLsizeiptr size,
          const T& initial_data = T{});
    Array(const Ctx& ctx,
          GLbitfield storage_flags,
          GLsizeiptr size,
          std::initializer_list<T> initial_data);
    Array(const Array& other) = delete;
    Array(Array&& other) = default;
    virtual ~Array();

    void setElement(std::size_t pos, const T& value);
    void copyElement(std::size_t src, std::size_t dst);
    T getElement(std::size_t pos) const;

    void setRange(std::size_t first, std::size_t count, const T* src);
    void copyRange(std::size_t src, std::size_t dst, std::size_t count);
    std::vector<T> getRange(std::size_t first, std::size_t count) const;

    T* map(GLenum access);
    T* mapRange(std::size_t first, std::size_t count, GLbitfield access);
    void flushRange(std::size_t offset, std::size_t count);
    void unmap();

    GLsizeiptr getSize() const;
    GLbitfield getStorageFlags() const;

private:
    GLsizeiptr size;
    GLbitfield storage_flags;
    T* mapped = nullptr;
    T* mapped_range = nullptr;

    static T* _alloc(GLsizeiptr n);
    static void _free(T* ptr);
};

template <IsTriviallyCopyable T>
Array<T>::Array(const Ctx& ctx,
                GLbitfield storage_flags,
                GLsizeiptr size,
                std::initializer_list<T> initial_data)
    : Buffer(ctx)
    , size(size)
    , storage_flags(storage_flags) {
    initStorage(size * sizeof(T), nullptr, storage_flags);

    if (storage_flags & GL_DYNAMIC_STORAGE_BIT) {
        if (storage_flags & GL_MAP_WRITE_BIT) {
            memcpy(map(GL_WRITE_ONLY), initial_data.begin(), size * sizeof(T));
            unmap();
        } else {
            setSubData(0, initial_data.size() * sizeof(T), initial_data.begin());
        }
    } else {
        Buffer tmp(ctx);
        tmp.initStorage(initial_data.size() * sizeof(T), initial_data.begin());
        tmp.copyTo(*this, 0, 0, initial_data.size() * sizeof(T));
    }
}

template <IsTriviallyCopyable T>
Array<T>::Array(const Ctx& ctx,
                GLbitfield storage_flags,
                GLsizeiptr size,
                const T& initial_data)
    : Buffer(ctx)
    , size(size)
    , storage_flags(storage_flags) {
    initStorage(size * sizeof(T), nullptr, storage_flags);

    if (storage_flags & GL_DYNAMIC_STORAGE_BIT) {
        if (storage_flags & GL_MAP_WRITE_BIT) {
            std::fill_n(map(GL_WRITE_ONLY), size, initial_data);
            unmap();
        } else {
            std::vector<T> initial(size, initial_data);
            setSubData(0, size * sizeof(T), initial.data());
        }
    } else {
        std::vector<T> initial(size, initial_data);

        Buffer tmp(ctx);
        tmp.initStorage(size * sizeof(T), initial.data(), 0);
        tmp.copyTo(*this, 0, 0, size * sizeof(T));
    }
}

template <IsTriviallyCopyable T>
Array<T>::~Array() {}

template <IsTriviallyCopyable T>
void Array<T>::setElement(std::size_t pos, const T& value) {
    if (pos >= getSize()) {
        throw std::out_of_range("");
    }
    setSubData(pos * sizeof(T), sizeof(T), &value);
}

template <IsTriviallyCopyable T>
void Array<T>::copyElement(std::size_t src, std::size_t dst) {
    if (src >= getSize() || dst >= getSize()) {
        throw std::out_of_range("");
    }
    if (src != dst) {
        copyTo(*this, src * sizeof(T), dst * sizeof(T), 1);
    }
}

template <IsTriviallyCopyable T>
T Array<T>::getElement(std::size_t pos) const {
    if (pos >= getSize()) {
        throw std::out_of_range("");
    }
    T result;
    getSubData(pos * sizeof(T), sizeof(T), &result);
    return result;
}

template <IsTriviallyCopyable T>
void Array<T>::setRange(std::size_t first, std::size_t count, const T* src) {
    if (first + count > getSize()) {
        throw std::out_of_range("");
    }
    setSubData(first * sizeof(T), count * sizeof(T), src);
}

template <IsTriviallyCopyable T>
void Array<T>::copyRange(std::size_t src, std::size_t dst, std::size_t count) {
    if (src + count > getSize() || dst + count > getSize()) {
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
std::vector<T> Array<T>::getRange(std::size_t first, std::size_t count) const {
    if (first + count > getSize()) {
        throw std::out_of_range("");
    }
    std::vector<T> result(count, T{});
    getSubData(first * sizeof(T), count * sizeof(T), result.data());
    return result;
}

template <IsTriviallyCopyable T>
GLsizeiptr Array<T>::getSize() const {
    return size;
}

template <IsTriviallyCopyable T>
GLbitfield Array<T>::getStorageFlags() const {
    return storage_flags;
}

template <IsTriviallyCopyable T>
T* Array<T>::map(GLenum access) {
    if (mapped_range) {
        throw std::runtime_error("Already mapped range");
    }

    if (!mapped) {
        mapped = reinterpret_cast<T*>(Buffer::map(access));
    }

    return mapped;
}

template <IsTriviallyCopyable T>
T* Array<T>::mapRange(std::size_t first, std::size_t count, GLbitfield access) {
    if (mapped) {
        throw std::runtime_error("Already mapped full");
    }

    if (!mapped_range) {
        mapped_range = reinterpret_cast<T*>(Buffer::mapRange(first * sizeof(T),
                                                             count * sizeof(T),
                                                             access));
    }

    return mapped_range;
}

template <IsTriviallyCopyable T>
void Array<T>::flushRange(std::size_t first, std::size_t count) {
    if (!mapped && !mapped_range) {
        throw std::runtime_error("Is not mapped");
    }
    Buffer::flushRange(first * sizeof(T), count * sizeof(T));
}

template <IsTriviallyCopyable T>
void Array<T>::unmap() {
    if (!mapped && !mapped_range) {
        throw std::runtime_error("Is not mapped");
    }
    Buffer::unmap();
    mapped = nullptr;
    mapped_range = nullptr;
}

} // namespace gnev::gl::buffer