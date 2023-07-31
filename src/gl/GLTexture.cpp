#include "gl/GLTexture.hpp"

using namespace gnev;

GLTexture::GLTexture(const std::shared_ptr<GladGLContext> &ctx, GLenum target) :
    GLHandler(ctx, create_handle(ctx, target), &handle_deleter)
    {
}

GLTexture::~GLTexture()
{
}

void GLTexture::glBindTexture(GLenum target) const
{
    ctx()->BindTexture(target, handle());
}

void GLTexture::glTextureParameteri(GLenum pname, GLint param)
{
    ctx()->TextureParameteri(handle(), pname, param);
}

void GLTexture::glGetTextureParameteriv(GLenum pname, GLint* params) const
{
    ctx()->GetTextureParameteriv(handle(), pname, params);
}

void GLTexture::glGetTextureLevelParameteriv(GLint level, GLenum pname, GLint* params) const
{
    ctx()->GetTextureLevelParameteriv(handle(), level, pname, params);
}

void GLTexture::glTextureParameterfv(GLenum pname, const GLfloat* param)
{
    ctx()->TextureParameterfv(handle(), pname, param);
}

void GLTexture::glTextureStorage3D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
    ctx()->TextureStorage3D(handle(), levels, internalformat, width, height, depth);
}

void GLTexture::glTextureSubImage3D(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)
{
    ctx()->TextureSubImage3D(handle(), level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
}

void GLTexture::glGenerateTextureMipmap()
{
    ctx()->GenerateTextureMipmap(handle());
}

void GLTexture::glCopyImageSubData(GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth) const
{
    ctx()->CopyImageSubData(handle(), srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);
}

void GLTexture::glGetTextureSubImage(GLint level,
                                     GLint xoffset, GLint yoffset, GLint zoffset,
                                     GLsizei width, GLsizei height, GLsizei depth,
                                     GLenum format, GLenum type, GLsizei bufSize, void *pixels) const
{
    ctx()->GetTextureSubImage(handle(), level, xoffset, yoffset, zoffset, width, height, depth, format, type, bufSize, pixels);
}

GLuint* GLTexture::create_handle(const std::shared_ptr<GladGLContext> &ctx, GLenum target)
{
    auto handle = new GLuint(0);
    if (ctx->VERSION_4_5)
    {
        ctx->CreateTextures(target, 1, handle);
    } else {
        ctx->GenTextures(1, handle);
        ctx->BindTexture(target, *handle);
    }
    return handle;
}

void GLTexture::handle_deleter(GLuint* handle, GladGLContext& ctx)
{
    ctx.DeleteTextures(1, handle);
    delete handle;
}