#pragma once

#include "gl/Handler.hpp"

namespace gnev::gl {

class EXPORT Texture : public Handler {
public:
    Texture(const Ctx& ctx, GLenum target);
    virtual ~Texture();

    void glBindTexture(GLenum target) const;
    void glTextureParameteri(GLenum pname, GLint param);
    void glGetTextureParameteriv(GLenum pname, GLint* params) const;
    void glGetTextureLevelParameteriv(GLint level, GLenum pname, GLint* params) const;
    void glTextureParameterfv(GLenum pname, const GLfloat* param);
    void glTextureStorage3D(GLsizei levels,
                            GLenum internalformat,
                            GLsizei width,
                            GLsizei height,
                            GLsizei depth);
    void glTextureSubImage3D(GLint level,
                             GLint xoffset,
                             GLint yoffset,
                             GLint zoffset,
                             GLsizei width,
                             GLsizei height,
                             GLsizei depth,
                             GLenum format,
                             GLenum type,
                             const void* pixels);
    void glGenerateTextureMipmap();
    void glCopyImageSubData(GLenum srcTarget,
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
                            GLsizei srcDepth) const;
    void glGetTextureSubImage(GLint level,
                              GLint xoffset,
                              GLint yoffset,
                              GLint zoffset,
                              GLsizei width,
                              GLsizei height,
                              GLsizei depth,
                              GLenum format,
                              GLenum type,
                              GLsizei bufSize,
                              void* pixels) const;

private:
    static GLuint createHandle(const Ctx& ctx, GLenum target);
    static void freeHandle(const Ctx& ctx, GLuint handle);
};

} // namespace gnev::gl