#include "gl/Texture.hpp"

using namespace gnev::gl;

Texture::Texture(const Ctx& ctx, GLenum target)
    : Handler(ctx, createHandle(ctx, target), &freeHandle) {}

Texture::~Texture() {}

void Texture::glBindTexture(GLenum target) const {
    ctx().glBindTexture(target, handle());
}

void Texture::glTextureParameteri(GLenum pname, GLint param) {
    ctx().glTextureParameteri(handle(), pname, param);
}

void Texture::glGetTextureParameteriv(GLenum pname, GLint* params) const {
    ctx().glGetTextureParameteriv(handle(), pname, params);
}

void Texture::glGetTextureLevelParameteriv(GLint level,
                                           GLenum pname,
                                           GLint* params) const {
    ctx().glGetTextureLevelParameteriv(handle(), level, pname, params);
}

void Texture::glTextureParameterfv(GLenum pname, const GLfloat* param) {
    ctx().glTextureParameterfv(handle(), pname, param);
}

void Texture::glTextureStorage3D(GLsizei levels,
                                 GLenum internalformat,
                                 GLsizei width,
                                 GLsizei height,
                                 GLsizei depth) {
    ctx().glTextureStorage3D(handle(), levels, internalformat, width, height, depth);
}

void Texture::glTextureSubImage3D(GLint level,
                                  GLint xoffset,
                                  GLint yoffset,
                                  GLint zoffset,
                                  GLsizei width,
                                  GLsizei height,
                                  GLsizei depth,
                                  GLenum format,
                                  GLenum type,
                                  const void* pixels) {
    ctx().glTextureSubImage3D(handle(),
                              level,
                              xoffset,
                              yoffset,
                              zoffset,
                              width,
                              height,
                              depth,
                              format,
                              type,
                              pixels);
}

void Texture::glGenerateTextureMipmap() { ctx().glGenerateTextureMipmap(handle()); }

void Texture::glCopyImageSubData(GLenum srcTarget,
                                 GLint srcLevel,
                                 GLint srcX,
                                 GLint srcY,
                                 GLint srcZ,
                                 GLuint dstName,
                                 GLenum dstTarget,
                                 GLint dstLevel,
                                 GLint dstX,
                                 GLint dstY,
                                 GLint dstZ,
                                 GLsizei srcWidth,
                                 GLsizei srcHeight,
                                 GLsizei srcDepth) const {
    ctx().glCopyImageSubData(handle(),
                             srcTarget,
                             srcLevel,
                             srcX,
                             srcY,
                             srcZ,
                             dstName,
                             dstTarget,
                             dstLevel,
                             dstX,
                             dstY,
                             dstZ,
                             srcWidth,
                             srcHeight,
                             srcDepth);
}

void Texture::glGetTextureSubImage(GLint level,
                                   GLint xoffset,
                                   GLint yoffset,
                                   GLint zoffset,
                                   GLsizei width,
                                   GLsizei height,
                                   GLsizei depth,
                                   GLenum format,
                                   GLenum type,
                                   GLsizei bufSize,
                                   void* pixels) const {
    ctx().glGetTextureSubImage(handle(),
                               level,
                               xoffset,
                               yoffset,
                               zoffset,
                               width,
                               height,
                               depth,
                               format,
                               type,
                               bufSize,
                               pixels);
}

GLuint Texture::createHandle(const Ctx& ctx, GLenum target) {
    GLuint handle;
    ctx.glCreateTextures(target, 1, &handle);
    return handle;
}

void Texture::freeHandle(const Ctx& ctx, GLuint handle) {
    ctx.glDeleteTextures(1, &handle);
}