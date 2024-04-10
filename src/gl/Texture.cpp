#include "gl/Texture.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/Enum.hpp"
#include "gl/logger/HandlerLogger.hpp"

using enum gnev::LogLevel;

namespace gnev::gl {

Texture::Texture(TextureTarget target)
    : Handler(createHandle(static_cast<GLenum>(target)), &deleteHandle) {}

Texture::~Texture() { getLogger().logFunc<L2>(); }

void Texture::bind(TextureTarget target) const {
    getLogger().logFunc<L2>(target);
    Ctx::Get().glBindTexture(static_cast<GLenum>(target), handle());
}

TextureTarget Texture::getTarget() const {
    getLogger().logFunc<L2>();
    TextureTarget target;
    Ctx::Get().glGetTextureParameteriv(handle(),
                                       GL_TEXTURE_TARGET,
                                       reinterpret_cast<GLint*>(&target));
    return target;
}

GLuint Texture::getWidth(GLuint level) const {
    getLogger().logFunc<L2>(level);
    GLint width;
    Ctx::Get().glGetTextureLevelParameteriv(handle(), level, GL_TEXTURE_WIDTH, &width);
    return width;
}

GLuint Texture::getHeight(GLuint level) const {
    getLogger().logFunc<L2>(level);
    GLint height;
    Ctx::Get().glGetTextureLevelParameteriv(handle(), level, GL_TEXTURE_HEIGHT, &height);
    return height;
}

GLuint Texture::getDepth(GLuint level) const {
    getLogger().logFunc<L2>(level);
    GLint depth;
    Ctx::Get().glGetTextureLevelParameteriv(handle(), level, GL_TEXTURE_DEPTH, &depth);
    return depth;
}

void Texture::setWrapS(TextureWrapS wrap) {
    getLogger().logFunc<L2>(wrap);
    Ctx::Get().glTextureParameteri(handle(),
                                   GL_TEXTURE_WRAP_S,
                                   static_cast<GLenum>(wrap));
}

void Texture::setWrapT(TextureWrapT wrap) {
    getLogger().logFunc<L2>(wrap);
    Ctx::Get().glTextureParameteri(handle(),
                                   GL_TEXTURE_WRAP_T,
                                   static_cast<GLenum>(wrap));
}

void Texture::setMinFilter(TextureMinFilter filter) {
    getLogger().logFunc<L2>(filter);
    Ctx::Get().glTextureParameteri(handle(),
                                   GL_TEXTURE_MIN_FILTER,
                                   static_cast<GLenum>(filter));
}

void Texture::setMagFilter(TextureMagFilter filter) {
    getLogger().logFunc<L2>(filter);
    Ctx::Get().glTextureParameteri(handle(),
                                   GL_TEXTURE_MAG_FILTER,
                                   static_cast<GLenum>(filter));
}

void Texture::generateMipmap() {
    getLogger().logFunc<L2>();
    Ctx::Get().glGenerateTextureMipmap(handle());
}

void Texture::initStorage3D(GLuint levels,
                            TextureInternalFormat internal_format,
                            GLuint width,
                            GLuint height,
                            GLuint depth) {
    getLogger().logFunc<L2>(levels, internal_format, width, height, depth);
    Ctx::Get().glTextureStorage3D(handle(),
                                  levels,
                                  static_cast<GLenum>(internal_format),
                                  width,
                                  height,
                                  depth);
}

void Texture::setSubImage3D(GLuint level,
                            GLuint x,
                            GLuint y,
                            GLuint z,
                            GLuint width,
                            GLuint height,
                            GLuint depth,
                            TextureFormat format,
                            TextureType type,
                            const void* pixels) {
    getLogger().logFunc<L2>(level, x, y, z, width, height, depth, format, type, pixels);
    Ctx::Get().glTextureSubImage3D(handle(),
                                   level,
                                   x,
                                   y,
                                   z,
                                   width,
                                   height,
                                   depth,
                                   static_cast<GLenum>(format),
                                   static_cast<GLenum>(type),
                                   pixels);
}

void Texture::getSubImage(GLuint level,
                          GLuint x,
                          GLuint y,
                          GLuint z,
                          GLuint width,
                          GLuint height,
                          GLuint depth,
                          TextureFormat format,
                          TextureType type,
                          GLuint bufSize,
                          void* pixels) const {
    getLogger()
        .logFunc<L2>(level, x, y, z, width, height, depth, format, type, bufSize, pixels);
    Ctx::Get().glGetTextureSubImage(handle(),
                                    level,
                                    x,
                                    y,
                                    z,
                                    width,
                                    height,
                                    depth,
                                    static_cast<GLenum>(format),
                                    static_cast<GLenum>(type),
                                    bufSize,
                                    pixels);
}

void Texture::copyTo(Texture& dst,
                     GLuint src_level,
                     GLuint src_x,
                     GLuint src_y,
                     GLuint src_z,
                     GLuint dst_level,
                     GLuint dst_x,
                     GLuint dst_y,
                     GLuint dst_z,
                     GLuint src_width,
                     GLuint src_height,
                     GLuint src_depth) const {
    getLogger().logFunc<L2>(dst.handle(),
                            src_level,
                            src_x,
                            src_y,
                            src_z,
                            dst_level,
                            dst_x,
                            dst_y,
                            dst_z,
                            src_width,
                            src_height,
                            src_depth);
    Ctx::Get().glCopyImageSubData(handle(),
                                  static_cast<GLenum>(getTarget()),
                                  src_level,
                                  src_x,
                                  src_y,
                                  src_z,
                                  dst.handle(),
                                  static_cast<GLenum>(dst.getTarget()),
                                  dst_level,
                                  dst_x,
                                  dst_y,
                                  dst_z,
                                  src_width,
                                  src_height,
                                  src_depth);
}

GLuint* Texture::createHandle(GLenum target) {
    auto handle = new GLuint(0);
    Ctx::Get().glCreateTextures(target, 1, handle);
    return handle;
}

void Texture::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteTextures(1, handle);
    delete handle;
}

} // namespace gnev::gl