#include "gl/Texture.hpp"

using namespace gnev::gl;

Texture::Texture(const std::shared_ptr<GladGLContext> &ctx, GLenum target) :
    Handler(ctx, create_handle(ctx, target), &handle_deleter)
    {
}

Texture::~Texture()
{
}

void Texture::glBindTexture(GLenum target) const
{
    ctx()->BindTexture(target, handle());
}

void Texture::glTextureParameteri(GLenum pname, GLint param)
{
    ctx()->TextureParameteri(handle(), pname, param);
}

void Texture::glGetTextureParameteriv(GLenum pname, GLint* params) const
{
    ctx()->GetTextureParameteriv(handle(), pname, params);
}

void Texture::glGetTextureLevelParameteriv(GLint level, GLenum pname, GLint* params) const
{
    ctx()->GetTextureLevelParameteriv(handle(), level, pname, params);
}

void Texture::glTextureParameterfv(GLenum pname, const GLfloat* param)
{
    ctx()->TextureParameterfv(handle(), pname, param);
}

void Texture::glTextureStorage3D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
    ctx()->TextureStorage3D(handle(), levels, internalformat, width, height, depth);
}

void Texture::glTextureSubImage3D(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)
{
    ctx()->TextureSubImage3D(handle(), level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
}

void Texture::glGenerateTextureMipmap()
{
    ctx()->GenerateTextureMipmap(handle());
}

void Texture::glCopyImageSubData(GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth) const
{
    ctx()->CopyImageSubData(handle(), srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);
}

void Texture::glGetTextureSubImage(GLint level,
                                     GLint xoffset, GLint yoffset, GLint zoffset,
                                     GLsizei width, GLsizei height, GLsizei depth,
                                     GLenum format, GLenum type, GLsizei bufSize, void *pixels) const
{
    ctx()->GetTextureSubImage(handle(), level, xoffset, yoffset, zoffset, width, height, depth, format, type, bufSize, pixels);
}

GLuint* Texture::create_handle(const std::shared_ptr<GladGLContext> &ctx, GLenum target)
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

void Texture::handle_deleter(GLuint* handle, GladGLContext& ctx)
{
    ctx.DeleteTextures(1, handle);
    delete handle;
}