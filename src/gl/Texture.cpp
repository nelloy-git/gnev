#include "gl/Texture.hpp"

using namespace gnev::gl;

Texture::Texture(GLenum target)
    : Handler(createHandle(target), &deleteHandle) {}

Texture::~Texture() {}

void Texture::bind(GLenum target) const { Ctx::Get().glBindTexture(target, handle()); }

void Texture::setParameteri(GLenum pname, GLint param) {
    Ctx::Get().glTextureParameteri(handle(), pname, param);
}

void Texture::getParameteriv(GLenum pname, GLint* params) const {
    Ctx::Get().glGetTextureParameteriv(handle(), pname, params);
}

void Texture::getLevelParameteriv(GLint level, GLenum pname, GLint* params) const {
    Ctx::Get().glGetTextureLevelParameteriv(handle(), level, pname, params);
}

void Texture::setParameterfv(GLenum pname, const GLfloat* param) {
    Ctx::Get().glTextureParameterfv(handle(), pname, param);
}

void Texture::generateMipmap() { Ctx::Get().glGenerateTextureMipmap(handle()); }

void Texture::initStorage3D(GLuint levels,
                            GLenum internalformat,
                            GLuint width,
                            GLuint height,
                            GLuint depth) {
    Ctx::Get().glTextureStorage3D(handle(), levels, internalformat, width, height, depth);
}

void Texture::initImage3D(GLint level,
                          GLint internalformat,
                          GLuint width,
                          GLuint height,
                          GLuint depth,
                          GLint border,
                          GLenum format,
                          GLenum type,
                          const void* pixels) {
    Ctx::Get().glTexImage3D(getTarget(),
                            level,
                            internalformat,
                            width,
                            height,
                            depth,
                            border,
                            format,
                            type,
                            pixels);
}

void Texture::setSubImage3D(GLuint level,
                            GLuint x,
                            GLuint y,
                            GLuint z,
                            GLuint width,
                            GLuint height,
                            GLuint depth,
                            GLenum format,
                            GLenum type,
                            const void* pixels) {
    Ctx::Get().glTextureSubImage3D(handle(),
                                   level,
                                   x,
                                   y,
                                   z,
                                   width,
                                   height,
                                   depth,
                                   format,
                                   type,
                                   pixels);
}

void Texture::getSubImage(GLuint level,
                          GLuint x,
                          GLuint y,
                          GLuint z,
                          GLuint width,
                          GLuint height,
                          GLuint depth,
                          GLenum format,
                          GLenum type,
                          GLuint bufSize,
                          void* pixels) const {
    Ctx::Get().glGetTextureSubImage(handle(),
                                    level,
                                    x,
                                    y,
                                    z,
                                    width,
                                    height,
                                    depth,
                                    format,
                                    type,
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
                     GLuint srcWidth,
                     GLuint srcHeight,
                     GLuint srcDepth) const {
    Ctx::Get().glCopyImageSubData(handle(),
                                  getTarget(),
                                  src_level,
                                  src_x,
                                  src_y,
                                  src_z,
                                  dst.handle(),
                                  dst.getTarget(),
                                  dst_level,
                                  dst_x,
                                  dst_y,
                                  dst_z,
                                  srcWidth,
                                  srcHeight,
                                  srcDepth);
}

GLenum Texture::getTarget() const {
    GLenum target;
    Ctx::Get().glGetTextureParameteriv(handle(),
                                       GL_TEXTURE_TARGET,
                                       reinterpret_cast<GLint*>(&target));
    return target;
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