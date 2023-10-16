#pragma once

#include "gl/Handler.hpp"

namespace gnev::gl {

class EXPORT Texture : public Handler {
public:
    Texture(GLenum target);
    Texture(const Texture&) = delete;
    Texture(Texture&&) = default;
    virtual ~Texture();

    void bind(GLenum target) const;
    void setParameteri(GLenum pname, GLint param);
    void getParameteriv(GLenum pname, GLint* params) const;
    void setParameterfv(GLenum pname, const GLfloat* param);
    void getLevelParameteriv(GLint level, GLenum pname, GLint* params) const;
    void generateMipmap();
    void initStorage3D(GLuint levels,
                       GLenum internalformat,
                       GLuint width,
                       GLuint height,
                       GLuint depth);
    void initImage3D(GLint level,
                     GLint internalformat,
                     GLuint width,
                     GLuint height,
                     GLuint depth,
                     GLint border,
                     GLenum format,
                     GLenum type,
                     const void* pixels);
    void setSubImage3D(GLuint level,
                       GLuint x,
                       GLuint y,
                       GLuint z,
                       GLuint width,
                       GLuint height,
                       GLuint depth,
                       GLenum format,
                       GLenum type,
                       const void* pixels);
    void getSubImage(GLuint level,
                     GLuint x,
                     GLuint y,
                     GLuint z,
                     GLuint width,
                     GLuint height,
                     GLuint depth,
                     GLenum format,
                     GLenum type,
                     GLuint bufSize,
                     void* pixels) const;
    void copyTo(Texture& dst,
                GLuint src_level,
                GLuint src_x,
                GLuint src_y,
                GLuint src_z,
                GLuint dstlevel,
                GLuint dstx,
                GLuint dsty,
                GLuint dstz,
                GLuint srcWidth,
                GLuint srcHeight,
                GLuint srcDepth) const;

    GLenum getTarget() const;

private:
    static GLuint* createHandle(GLenum target);
    static void deleteHandle(GLuint* handle);
};

} // namespace gnev::gl