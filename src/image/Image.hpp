#pragma once

#include "gl/Ctx.hpp"
#include "gl/enum/TextureFormat.hpp"
#include "gl/enum/TextureType.hpp"
#include "util/Export.hpp"
#include "util/Ref.hpp"

namespace gnev {

struct EXPORT ImageInfo {
    GLuint level = 0;
    GLuint x = 0;
    GLuint y = 0;
    GLuint width = 0;
    GLuint height = 0;
    TextureFormat format;
    TextureType type;

    GLuint calcTextureBufferSize() const;
    GLuint getBytesPerPixel() const;
    GLuint getComponents() const;
};

struct EXPORT ImageInfo3d {
    ImageInfo3d(GLuint z, const ImageInfo& info);

    GLuint level = 0;
    GLuint x = 0;
    GLuint y = 0;
    GLuint z = 0;
    GLuint width = 0;
    GLuint height = 0;
    GLuint depth = 0;
    TextureFormat format;
    TextureType type;

    GLuint calcTextureBufferSize() const;
    GLuint getBytesPerPixel() const;
    GLuint getComponents() const;
};

struct ImageData {
    ImageData(const ImageData& other) = default;
    ImageData(ImageData&& other) = default;
    ImageData& operator=(const ImageData& other) = default;
    ImageData& operator=(ImageData&& other) = default;

    ImageData(std::size_t size)
        : buffer_size(size)
        , buffer(std::make_shared<GLubyte[]>(size)) {}

    ImageData(std::size_t size, const std::shared_ptr<GLubyte[]> buffer)
        : buffer_size(size)
        , buffer(buffer) {}

    template <typename T = void>
    T* get() const {
        return buffer.get();
    }

    std::size_t buffer_size;
    std::shared_ptr<GLubyte[]> buffer;
};

struct Image {
    Image(const ImageInfo& info, std::size_t size)
        : info(info)
        , data(size) {}

    Image(const ImageInfo& info, const ImageData& data)
        : info(info)
        , data(data) {}

    ImageInfo info;
    ImageData data;
};

struct Image3d {
    Image3d(const ImageInfo3d& info, std::size_t size)
        : info(info)
        , data(size) {}

    Image3d(const ImageInfo3d& info, const ImageData& data)
        : info(info)
        , data(data) {}

    ImageInfo3d info;
    ImageData data;
};

} // namespace gnev