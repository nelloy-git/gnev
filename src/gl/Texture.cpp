#include "gl/Texture.hpp"

using namespace gnev::gl;

Texture::Texture(const Ctx& ctx, GLenum target)
    : Handler(ctx, createHandle(ctx, target), &freeHandle) {}

Texture::~Texture() {}

void Texture::bind(GLenum target) const { ctx().glBindTexture(target, handle()); }

void Texture::setParameteri(GLenum pname, GLint param) {
    ctx().glTextureParameteri(handle(), pname, param);
}

void Texture::getParameteriv(GLenum pname, GLint* params) const {
    ctx().glGetTextureParameteriv(handle(), pname, params);
}

void Texture::getLevelParameteriv(GLint level, GLenum pname, GLint* params) const {
    ctx().glGetTextureLevelParameteriv(handle(), level, pname, params);
}

void Texture::setParameterfv(GLenum pname, const GLfloat* param) {
    ctx().glTextureParameterfv(handle(), pname, param);
}

void Texture::generateMipmap() { ctx().glGenerateTextureMipmap(handle()); }

void Texture::initStorage3D(GLsizei levels,
                            GLenum internalformat,
                            GLsizei width,
                            GLsizei height,
                            GLsizei depth) {
    ctx().glTextureStorage3D(handle(), levels, internalformat, width, height, depth);
}

void Texture::initImage3D(GLint level,
                          GLint internalformat,
                          GLsizei width,
                          GLsizei height,
                          GLsizei depth,
                          GLint border,
                          GLenum format,
                          GLenum type,
                          const void* pixels) {
    ctx().glTexImage3D(getTarget(),
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

void Texture::setSubImage3D(Pos pos,
                            GLsizei width,
                            GLsizei height,
                            GLsizei depth,
                            GLenum format,
                            GLenum type,
                            const void* pixels) {
    ctx().glTextureSubImage3D(handle(),
                              pos.level,
                              pos.x,
                              pos.y,
                              pos.z,
                              width,
                              height,
                              depth,
                              format,
                              type,
                              pixels);
}

void Texture::getSubImage(const Pos& pos,
                          GLsizei width,
                          GLsizei height,
                          GLsizei depth,
                          GLenum format,
                          GLenum type,
                          GLsizei bufSize,
                          void* pixels) const {
    ctx().glGetTextureSubImage(handle(),
                               pos.level,
                               pos.x,
                               pos.y,
                               pos.z,
                               width,
                               height,
                               depth,
                               format,
                               type,
                               bufSize,
                               pixels);
}

void Texture::copyTo(Texture& dst,
                     const Pos& src_pos,
                     const Pos& dst_pos,
                     GLsizei srcWidth,
                     GLsizei srcHeight,
                     GLsizei srcDepth) const {
    ctx().glCopyImageSubData(handle(),
                             getTarget(),
                             src_pos.level,
                             src_pos.x,
                             src_pos.y,
                             src_pos.z,
                             dst.handle(),
                             dst.getTarget(),
                             dst_pos.level,
                             dst_pos.x,
                             dst_pos.y,
                             dst_pos.z,
                             srcWidth,
                             srcHeight,
                             srcDepth);
}

GLenum Texture::getTarget() const {
    GLenum target;
    ctx().glGetTextureParameteriv(handle(),
                                  GL_TEXTURE_TARGET,
                                  reinterpret_cast<GLint*>(&target));
    return target;
}

GLuint Texture::createHandle(const Ctx& ctx, GLenum target) {
    GLuint handle;
    ctx.glCreateTextures(target, 1, &handle);
    return handle;
}

void Texture::freeHandle(const Ctx& ctx, GLuint handle) {
    ctx.glDeleteTextures(1, &handle);
}