#pragma once

#include <memory>
#include <string>

#include "glad/gl.h"

#include "util/Util.hpp"

namespace gnev {

class EXPORT GLShader final {
public:
    GLShader(const std::shared_ptr<GladGLContext> &ctx, GLenum type);
    GLShader(const GLShader&) = delete;
    GLShader(GLShader&&) = delete;
    ~GLShader();

    void glShaderSource(GLsizei count, const GLchar *const *string, const GLint *length);
    void glCompileShader();
    void glGetShaderiv(GLenum pname, GLint* params) const;
    void glGetShaderInfoLog(GLsizei bufSize, GLsizei *length, GLchar *infoLog) const;

    std::pair<bool, std::string> compile_with_info_log(const std::string& src);

    const std::shared_ptr<GladGLContext> ctx;
    const GLuint handle;

private:
    static GLuint create_shader(const std::shared_ptr<GladGLContext> &ctx, GLenum type);

};

}