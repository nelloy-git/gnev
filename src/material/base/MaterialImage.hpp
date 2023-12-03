#pragma once

#include <memory>

#include "gl/Texture.hpp"

namespace gnev::gl {

struct TexImageInfo {
    GLuint level = 0;
    GLuint x = 0;
    GLuint y = 0;
    GLuint width = 0;
    GLuint height = 0;
    GLenum format;
    GLenum type;
};

struct TexImageData {
    TexImageData() = default;

    TexImageData(std::size_t size)
        : buffer_size(size)
        , buffer(std::make_shared<GLubyte[]>(size)) {}

    TexImageData(std::size_t size, const std::shared_ptr<void> buffer)
        : buffer_size(size)
        , buffer(buffer) {}

    std::size_t size() const { return buffer_size; };

    template <typename T = void>
    std::shared_ptr<T> share() {
        return std::reinterpret_pointer_cast<T>(buffer);
    }

    template <typename T = void>
    std::shared_ptr<const T> share() const {
        return std::reinterpret_pointer_cast<const T>(buffer);
    }

    template <typename T = void>
        requires(!std::is_array_v<T>)
    T* data() {
        return reinterpret_cast<T*>(buffer.get());
    }

    template <typename T = void>
        requires(!std::is_array_v<T>)
    const T* data() const {
        return reinterpret_cast<const T*>(buffer.get());
    }

private:
    std::size_t buffer_size = 0;
    std::shared_ptr<void> buffer = nullptr;
};

struct TexImage {
    TexImage() {}

    TexImage(const TexImageInfo& info, std::size_t size)
        : info(info)
        , data(size) {}

    TexImage(const TexImageInfo& info, const TexImageData& data)
        : info(info)
        , data(data) {}

    TexImageInfo info;
    TexImageData data;
};

} // namespace gnev::gl