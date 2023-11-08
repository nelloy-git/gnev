#pragma once

#include <memory>

#include "gl/Texture.hpp"

namespace gnev::gl::texture {

struct ImageInfo {
    GLuint level = 0;
    GLuint x = 0;
    GLuint y = 0;
    GLuint width = 0;
    GLuint height = 0;
    GLenum format;
    GLenum type;
};

struct ImageData {
    ImageData() = default;

    ImageData(std::size_t size)
        : buffer_size(size)
        , buffer(std::make_shared<GLubyte[]>(size)) {}

    ImageData(std::size_t size, const std::shared_ptr<void> buffer)
        : buffer_size(size)
        , buffer(buffer) {}

    std::size_t size() const { return buffer_size; };

    template <typename T = void>
    std::shared_ptr<T> share() {
        return std::reinterpret_pointer_cast<T>(buffer);
    }

    template <typename T = void>
    std::shared_ptr<const T> share() {
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

struct Image {
    Image() {}

    Image(const ImageInfo& info, std::size_t size)
        : info(info)
        , data(size) {}

    Image(const ImageInfo& info, const ImageData& data)
        : info(info)
        , data(data) {}

    ImageInfo info;
    ImageData data;
};

} // namespace gnev::gl::texture