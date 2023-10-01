#pragma once

#include "gl/Handler.hpp"

namespace gnev::gl {

class EXPORT Program : public Handler {
public:
    Program(const Ctx& ctx);
    Program(const Program& other) = delete;
    Program(Program&& other) = default;
    virtual ~Program();

    void glAttachShader(GLuint shader);
    void glValidateProgram();
    void glLinkProgram();
    void glUseProgram() const;
    void glGetProgramiv(GLenum pname, GLint* params) const;
    void glGetProgramInfoLog(GLsizei bufSize, GLsizei* length, GLchar* infoLog) const;
    GLint glGetUniformBlockIndex(const GLchar* uniformBlockName) const;
    void glUniformBlockBinding(GLuint uniformBlockIndex,
                               GLuint uniformBlockBinding) const;
    GLint glGetAttribLocation(const GLchar* name) const;
    GLint glGetUniformLocation(const GLchar* name) const;

    GLint glGetProgramResourceIndex(GLenum programInterface, const GLchar* name) const;
    void glShaderStorageBlockBinding(GLuint storageBlockIndex,
                                     GLuint storageBlockBinding) const;

private:
    static GLuint createHandle(const Ctx& ctx);
    static void freeHandle(const Ctx& ctx, GLuint handle);
};

} // namespace gnev::gl