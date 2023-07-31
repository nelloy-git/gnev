#pragma once

#include "gl/GLTexture.hpp"

#ifndef GNEV_GL_TEXTURE_VECTOR_CAP_BASE
#define GNEV_GL_TEXTURE_VECTOR_CAP_BASE 1
#endif

#ifndef GNEV_GL_TEXTURE_VECTOR_CAP_MULT
#define GNEV_GL_TEXTURE_VECTOR_CAP_MULT 2
#endif

#ifndef GNEV_GL_TEXTURE_VECTOR_CAP_ADD
#define GNEV_GL_TEXTURE_VECTOR_CAP_ADD 0
#endif

namespace gnev {

class EXPORT GLTextureVector : public GLTexture {

public:
    static constexpr GLsizeiptr CAP_BASE = GNEV_GL_TEXTURE_VECTOR_CAP_BASE;
    static constexpr GLsizeiptr CAP_MULT = GNEV_GL_TEXTURE_VECTOR_CAP_MULT;
    static constexpr GLsizeiptr CAP_ADD = GNEV_GL_TEXTURE_VECTOR_CAP_ADD;

    GLTextureVector(const std::shared_ptr<GladGLContext>& ctx, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
    virtual ~GLTextureVector();

    GLsizeiptr size() const;
    GLsizeiptr capacity() const;
    GLsizeiptr max_size() const;

    void set(GLsizei i, GLint level, GLenum format, GLenum type, const void* img);
    void push_back(GLint level, GLenum format, GLenum type, const void* img);
    void reserve(GLsizei capacity);

private:
    GLsizei _levels;
    GLenum _internalformat;
    GLsizei _size;
    GLsizei _cap;

    GLint get_level_parameteri(GLint level, GLenum pname) const;

};

}