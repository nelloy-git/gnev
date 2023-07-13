#pragma once

#include <memory>
#include <string>

#include "glad/gl.h"

#include "util/Util.hpp"

namespace gnev {

class EXPORT GLProgram final {
public:
    GLProgram(const std::shared_ptr<GladGLContext> &ctx);
    GLProgram(const GLProgram&) = delete;
    GLProgram(GLProgram&&) = delete;
    ~GLProgram();

    void glAttachShader(GLuint shader);
    void glValidateProgram();
    void glLinkProgram();
    void glUseProgram();
    void glGetProgramiv(GLenum pname, GLint *params) const;
    void glGetProgramInfoLog(GLsizei bufSize, GLsizei *length, GLchar *infoLog) const;
    GLuint glGetUniformBlockIndex(const GLchar *uniformBlockName) const;
    GLint glGetAttribLocation(const GLchar* name) const;

    std::pair<bool, std::string> validate_with_info_log();
    std::pair<bool, std::string> link_with_info_log();

    const std::shared_ptr<GladGLContext> ctx;
    const GLuint handle;

private:
    static GLuint create_program(const std::shared_ptr<GladGLContext> &ctx);

};

}