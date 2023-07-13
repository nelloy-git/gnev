#include "gl/GLTexture.hpp"

using namespace gnev;

GLTexture::GLTexture(const std::shared_ptr<GladGLContext> &ctx, GLenum target) :
    handle(create_texture(ctx, target)),
    ctx(ctx){
}

GLTexture::~GLTexture(){
    ctx->DeleteTextures(1, &handle);
}

void GLTexture::glTextureParameteri(GLenum pname, GLint param){
    ctx->TextureParameteri(handle, pname, param);
}

void GLTexture::glTextureParameterfv(GLenum pname, const GLfloat* param){
    ctx->TextureParameterfv(handle, pname, param);
}

void GLTexture::glTextureStorage3D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth){
    ctx->TextureStorage3D(handle, levels, internalformat, width, height, depth);
}

void GLTexture::glTextureSubImage3D(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels){
    ctx->TextureSubImage3D(handle, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
}

void GLTexture::glGenerateTextureMipmap(){
    ctx->GenerateTextureMipmap(handle);
}

void GLTexture::glCopyImageSubData(GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth) const {
    ctx->CopyImageSubData(handle, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);
}

GLuint GLTexture::create_texture(const std::shared_ptr<GladGLContext> &ctx, GLenum target){
    GLuint handle;
    if (ctx->VERSION_4_5){
        ctx->CreateTextures(target, 1, &handle);
    } else {
        ctx->GenTextures(1, &handle);
        ctx->BindTexture(target, handle);
    }
    return handle;
}
