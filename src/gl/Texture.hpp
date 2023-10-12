#pragma once

#include "gl/Handler.hpp"

namespace gnev::gl {

class EXPORT Texture : public Handler {
public:
    struct Pos {
        GLint level;
        GLint x;
        GLint y;
        GLint z;
    };

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
    void initStorage3D(GLsizei levels,
                       GLenum internalformat,
                       GLsizei width,
                       GLsizei height,
                       GLsizei depth);
    void initImage3D(GLint level,
                     GLint internalformat,
                     GLsizei width,
                     GLsizei height,
                     GLsizei depth,
                     GLint border,
                     GLenum format,
                     GLenum type,
                     const void* pixels);
    void setSubImage3D(Pos pos,
                       GLsizei width,
                       GLsizei height,
                       GLsizei depth,
                       GLenum format,
                       GLenum type,
                       const void* pixels);
    void getSubImage(const Pos& pos,
                     GLsizei width,
                     GLsizei height,
                     GLsizei depth,
                     GLenum format,
                     GLenum type,
                     GLsizei bufSize,
                     void* pixels) const;
    void copyTo(Texture& dst,
                const Pos& src_pos,
                const Pos& dst_pos,
                GLsizei srcWidth,
                GLsizei srcHeight,
                GLsizei srcDepth) const;

    GLenum getTarget() const;

private:
    static GLuint* createHandle(GLenum target);
    static void deleteHandle(GLuint* handle);
};

} // namespace gnev::gl