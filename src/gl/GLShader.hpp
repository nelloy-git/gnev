#pragma once

#include "gl/GLHandler.hpp"

namespace gnev {

class EXPORT GLShader : public GLHandler {
public:
    GLShader(const std::shared_ptr<GladGLContext> &ctx, GLenum type);
    virtual ~GLShader();

    void glShaderSource(GLsizei count, const GLchar *const *string, const GLint *length);
    void glCompileShader();
    void glGetShaderiv(GLenum pname, GLint* params) const;
    void glGetShaderInfoLog(GLsizei bufSize, GLsizei *length, GLchar *infoLog) const;

private:
    static GLuint* create_handle(const std::shared_ptr<GladGLContext> &ctx, GLenum type);
    static void handle_deleter(GLuint* handle, GladGLContext& ctx);

};

}