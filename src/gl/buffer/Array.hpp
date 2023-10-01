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

    GLsizeiptr getSize() const;
    GLbitfield getStorageFlags() const;

private:
    GLsizeiptr size;

    static T* _alloc(GLsizeiptr n);
    static void _free(T* ptr);
};

template <IsTriviallyCopyable T>
Array<T>::Array(const Ctx& ctx,
                GLbitfield storage_flags,
                GLsizeiptr size,
                std::initializer_list<T> initial_data)
    : Buffer(ctx)
    , size(size) {
    glBufferStorage(size * sizeof(T), nullptr, storage_flags);

    if (storage_flags & GL_DYNAMIC_STORAGE_BIT) {
        if (storage_flags & GL_MAP_WRITE_BIT) {
            T* ptr = static_cast<T*>(glMapBuffer(GL_WRITE_ONLY));
            memcpy(ptr, initial_data.begin(), size * sizeof(T));
            glUnmapBuffer();
        } else {
            glBufferSubData(0, initial_data.size() * sizeof(T), initial_data.begin());
        }
    } else {
        Buffer tmp(ctx);
        tmp.glBufferStorage(initial_data.size() * sizeof(T), initial_data.begin());
        tmp.glCopyBufferSubData(handle(), 0, 0, initial_data.size() * sizeof(T));
    }
}

template <IsTriviallyCopyable T>
Array<T>::Array(const Ctx& ctx,
                GLbitfield storage_flags,
                GLsizeiptr size,
                const T& initial_data)
    : Buffer(ctx)
    , size(size) {
    glBufferStorage(size * sizeof(T), nullptr, storage_flags);

    if (storage_flags & GL_DYNAMIC_STORAGE_BIT) {
        if (storage_flags & GL_MAP_WRITE_BIT) {
            T* ptr = static_cast<T*>(glMapBuffer(GL_WRITE_ONLY));
            std::fill_n(ptr, size, initial_data);
            glUnmapBuffer();
        } else {
            std::vector<T> initial(size, initial_data);
            glBufferSubData(0, size * sizeof(T), initial.data());
        }
    } else {
        std::vector<T> initial(size, initial_data);

        Buffer tmp(ctx);
        tmp.glBufferStorage(size * sizeof(T), initial.data(), 0);
        tmp.glCopyBufferSubData(handle(), 0, 0, size * sizeof(T));
    }
}

template <IsTriviallyCopyable T>
Array<T>::~Array() {}

template <IsTriviallyCopyable T>
void Array<T>::setElement(std::size_t pos, const T& value) {
    if (pos >= getSize()) {
        throw std::out_of_range("");
    }
    glBufferSubData(pos * sizeof(T), sizeof(T), &value);
}

template <IsTriviallyCopyable T>
void Array<T>::copyElement(std::size_t src, std::size_t dst) {
    if (src >= getSize() || dst >= getSize()) {
        throw std::out_of_range("");
    }
    if (src != dst) {
        glCopyBufferSubData(handle(), src * sizeof(T), dst * sizeof(T), 1);
    }
}

template <IsTriviallyCopyable T>
T Array<T>::getElement(std::size_t pos) const {
    if (pos >= getSize()) {
        throw std::out_of_range("");
    }
    T result;
    glGetBufferSubData(pos * sizeof(T), sizeof(T), &result);
    return result;
}

template <IsTriviallyCopyable T>
void Array<T>::setRange(std::size_t first, std::size_t count, const T* src) {
    if (first + count > getSize()) {
        throw std::out_of_range("");
    }
    glBufferSubData(first * sizeof(T), count * sizeof(T), src);
}

template <IsTriviallyCopyable T>
void Array<T>::copyRange(std::size_t src, std::size_t dst, std::size_t count) {
    if (src + count > getSize() || dst + count > getSize()) {
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
std::vector<T> Array<T>::getRange(std::size_t first, std::size_t count) const {
    if (first + count > getSize()) {
        throw std::out_of_range("");
    }
    std::vector<T> result(count, T{});
    glGetBufferSubData(first * sizeof(T), count * sizeof(T), result.data());
    return result;
}

template <IsTriviallyCopyable T>
GLsizeiptr Array<T>::getSize() const {
    return size;
}

template <IsTriviallyCopyable T>
GLbitfield Array<T>::getStorageFlags() const {
    GLbitfield flags;
    glGetBufferParameteriv(GL_BUFFER_STORAGE_FLAGS, reinterpret_cast<GLint*>(&flags));
    return flags;
}

} // namespace gnev::gl::buffer