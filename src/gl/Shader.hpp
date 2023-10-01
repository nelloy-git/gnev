#pragma once

#include "gl/Handler.hpp"

namespace gnev::gl {

class EXPORT Shader : public Handler {
public:
    Shader(const Ctx& ctx, GLenum type);
    virtual ~Shader();

    void glShaderSource(GLsizei count, const GLchar* const* string, const GLint* length);
    void glCompileShader();
    void glGetShaderiv(GLenum pname, GLint* params) const;
    void glGetShaderInfoLog(GLsizei bufSize, GLsizei* length, GLchar* infoLog) const;

private:
    static GLuint createHandle(const Ctx& ctx, GLenum type);
    static void freeHandle(const Ctx& ctx, GLuint handle);
};

} // namespace gnev::gl