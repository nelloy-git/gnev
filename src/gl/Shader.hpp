#pragma once

#include "gl/Handler.hpp"

namespace gnev::gl {

class EXPORT Shader : public Handler {
public:
    Shader(GLenum type);
    virtual ~Shader();

    void glShaderSource(GLsizei count, const GLchar* const* string, const GLint* length);
    void glCompileShader();
    void glGetShaderiv(GLenum pname, GLint* params) const;
    void glGetShaderInfoLog(GLsizei bufSize, GLsizei* length, GLchar* infoLog) const;

private:
    static GLuint* createHandle(GLenum type);
    static void deleteHandle(GLuint* handle);
};

} // namespace gnev::gl