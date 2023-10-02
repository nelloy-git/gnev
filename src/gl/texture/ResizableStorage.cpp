#include "gl/texture/ResizableStorage.hpp"

#include <memory>
#include <stdexcept>

using namespace gnev::gl;
using namespace gnev::gl::texture;

ResizableStorage::ResizableStorage(const Ctx& ctx,
                                   std::size_t levels,
                                   GLenum internalformat,
                                   std::size_t width,
                                   std::size_t height,
                                   std::size_t initial_capacity)
    : Texture(ctx, GL_TEXTURE_2D_ARRAY)
    , levels(levels)
    , capacity(initial_capacity) {
    // initStorage3D(levels, internalformat, width, height, capacity);
    initImage3D(0,
                internalformat,
                width,
                height,
                initial_capacity,
                0,
                internalformat,
                GL_UNSIGNED_BYTE,
                nullptr);
    constexpr int a = GL_RGBA;
}

ResizableStorage::~ResizableStorage() {}

void ResizableStorage::setElement(std::size_t pos, const Image& img) {
    if (pos >= capacity || img.level >= getLevels()) {
        throw std::out_of_range("");
    }

    setSubImage3D({GLint(img.level), GLint(img.x), GLint(img.y), GLint(pos)},
                  img.width,
                  img.height,
                  1,
                  img.format,
                  img.type,
                  img.data.get());
}

void ResizableStorage::copyElement(std::size_t src, std::size_t dst, std::size_t level) {
    copyRange(src, dst, 1);
}

Image ResizableStorage::getElement(std::size_t pos,
                                   std::size_t level,
                                   GLenum format,
                                   GLenum type) const {
    if (pos >= capacity || level >= getLevels()) {
        throw std::out_of_range("");
    }

    std::size_t buffer_size = getLevelBufferSize(level, format, type);

    Image result{
        .level = level,
        .x = 0,
        .y = 0,
        .width = getLevelWidth(level),
        .height = getLevelHeight(level),
        .format = format,
        .type = type,
        .data = std::shared_ptr<char[]>(new char[buffer_size]),
    };

    getSubImage({GLint(level), 0, 0, GLint(pos)},
                result.width,
                result.height,
                1,
                result.format,
                result.type,
                buffer_size,
                result.data.get());
    return result;
}

void ResizableStorage::copyRange(std::size_t src, std::size_t dst, std::size_t count) {
    if (src + count > capacity || dst + count > capacity) {
        throw std::out_of_range("");
    }

    if (src == dst) {
        return;
    }

    if (src >= dst + count || dst >= src + count) {
        for (std::size_t level = 0; level < getLevels(); ++level) {
            copyTo(*this,
                   {GLint(level), 0, 0, GLint(src)},
                   {GLint(level), 0, 0, GLint(dst)},
                   getLevelWidth(level),
                   getLevelHeight(level),
                   count);
        }
    } else {
        Texture tmp(ctx(), GL_TEXTURE_2D_ARRAY);
        tmp.initStorage3D(getLevels(),
                          getLevelInternalFormat(0),
                          getLevelWidth(0),
                          getLevelHeight(0),
                          count);

        for (std::size_t level = 0; level < getLevels(); ++level) {
            copyTo(tmp,
                   {GLint(level), 0, 0, GLint(src)},
                   {GLint(level), 0, 0, 0},
                   getLevelWidth(level),
                   getLevelHeight(level),
                   count);
        }

        for (std::size_t level = 0; level < getLevels(); ++level) {
            tmp.copyTo(*this,
                       {GLint(level), 0, 0, 0},
                       {GLint(level), 0, 0, GLint(dst)},
                       getLevelWidth(level),
                       getLevelHeight(level),
                       count);
        }
    }
}

std::size_t ResizableStorage::getLevels() const { return levels; }

GLenum ResizableStorage::getLevelInternalFormat(std::size_t level) const {
    GLenum internal_format;
    getLevelParameteriv(level,
                        GL_TEXTURE_INTERNAL_FORMAT,
                        reinterpret_cast<GLint*>(&internal_format));
    return internal_format;
}

std::size_t ResizableStorage::getLevelWidth(std::size_t level) const {
    GLint width;
    getLevelParameteriv(level, GL_TEXTURE_WIDTH, &width);
    return width;
}

std::size_t ResizableStorage::getLevelHeight(std::size_t level) const {
    GLint height;
    getLevelParameteriv(level, GL_TEXTURE_HEIGHT, &height);
    return height;
}

std::size_t ResizableStorage::getLevelBufferSize(std::size_t level,
                                                 GLenum format,
                                                 GLenum type) const {
    float elements = 0;
    switch (format) {
    case GL_RED:
        elements = 1;
        break;
    case GL_RG:
        elements = 2;
        break;
    case GL_RGB:
        elements = 3;
        break;
    case GL_BGR:
        elements = 3;
        break;
    case GL_RGBA:
        elements = 4;
        break;
    case GL_BGRA:
        elements = 4;
        break;
    case GL_DEPTH_COMPONENT:
        elements = 1;
        break;
    case GL_STENCIL_INDEX:
        elements = 1;
        break;
    default:
        throw std::out_of_range("");
    }

    float elem_size = 0;
    switch (type) {
    case GL_UNSIGNED_BYTE:
        elem_size = sizeof(GLubyte);
        break;
    case GL_BYTE:
        elem_size = sizeof(GLbyte);
        break;
    case GL_UNSIGNED_SHORT:
        elem_size = sizeof(GLushort);
        break;
    case GL_SHORT:
        elem_size = sizeof(GLshort);
        break;
    case GL_UNSIGNED_INT:
        elem_size = sizeof(GLuint);
        break;
    case GL_INT:
        elem_size = sizeof(GLint);
        break;
    case GL_FLOAT:
        elem_size = sizeof(GLfloat);
        break;
    case GL_UNSIGNED_BYTE_3_3_2:
    case GL_UNSIGNED_BYTE_2_3_3_REV:
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_5_6_5_REV:
        elem_size = sizeof(GLushort) / 3;
        break;
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_4_4_4_4_REV:
    case GL_UNSIGNED_SHORT_5_5_5_1:
    case GL_UNSIGNED_SHORT_1_5_5_5_REV:
        elem_size = sizeof(GLushort) / 4;
        break;
    case GL_UNSIGNED_INT_8_8_8_8:
    case GL_UNSIGNED_INT_8_8_8_8_REV:
    case GL_UNSIGNED_INT_10_10_10_2:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
        elem_size = sizeof(GLuint) / 4;
        break;
    default:
        throw std::out_of_range("");
    }

    return getLevelWidth(level) * getLevelHeight(level) * (elements * elem_size);
}

void ResizableStorage::setCapacity(std::size_t cap) {
    if (cap > getMaxLayers()) {
        throw std::out_of_range("");
    }

    Texture tmp(ctx(), GL_TEXTURE_2D_ARRAY);
    tmp.initStorage3D(getLevels(),
                      getLevelInternalFormat(0),
                      getLevelWidth(0),
                      getLevelHeight(0),
                      capacity);

    for (std::size_t level = 0; level < getLevels(); ++level) {
        copyTo(tmp,
               {GLint(level), 0, 0, 0},
               {GLint(level), 0, 0, 0},
               getLevelWidth(level),
               getLevelHeight(level),
               capacity);
    }

    initStorage3D(getLevels(),
                  getLevelInternalFormat(0),
                  getLevelWidth(0),
                  getLevelHeight(0),
                  cap);
    for (std::size_t level = 0; level < getLevels(); ++level) {
        tmp.copyTo(*this,
                   {GLint(level), 0, 0, 0},
                   {GLint(level), 0, 0, 0},
                   getLevelWidth(level),
                   getLevelHeight(level),
                   capacity);
    }

    capacity = cap;
}

std::size_t ResizableStorage::getCapacity() const { return capacity; }

std::size_t ResizableStorage::getMaxLayers() const {
    GLint size;
    ctx().glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &size);
    return size;
}

GLenum ResizableStorage::selectFormat(GLenum internal_format) const {
    switch (internal_format) {
    case GL_DEPTH_COMPONENT:
        return GL_DEPTH_COMPONENT;
    case GL_DEPTH_STENCIL:
        return GL_DEPTH_STENCIL;
    case GL_RED:
        return GL_RED;
    case GL_RG:
        return GL_RG;
    case GL_RGB:
        return GL_RGB;
    case GL_RGBA:
        return GL_RGBA;

    default:
        throw std::out_of_range("");
    }
}