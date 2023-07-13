#pragma once

#include <memory>

#include "glad/gl.h"

#include "util/Util.hpp"

namespace gnev {

class EXPORT GLTexture final {
public:
    GLTexture(const std::shared_ptr<GladGLContext> &ctx, GLenum target);
    GLTexture(const GLTexture&) = delete;
    GLTexture(GLTexture&&) = delete;
    ~GLTexture();

    void glTextureParameteri(GLenum pname, GLint param);
    void glTextureParameterfv(GLenum pname, const GLfloat* param);
    void glTextureStorage3D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
    void glTextureSubImage3D(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
    void glGenerateTextureMipmap();
    void glCopyImageSubData(GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth) const;

    const std::shared_ptr<GladGLContext> ctx;
    const GLuint handle;

private:
    static GLuint create_texture(const std::shared_ptr<GladGLContext> &ctx, GLenum target);
};

}