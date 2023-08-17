#include "gl/Program.hpp"

using namespace gnev::gl;

Program::Program(const GladCtx& ctx) :
    Handler(ctx, create_handle(ctx), &handle_deleter)
{
}

Program::~Program()
{
}

void Program::glAttachShader(GLuint shader)
{
    ctx()->AttachShader(handle(), shader);
}

void Program::glValidateProgram()
{
    ctx()->ValidateProgram(handle());
}

void Program::glLinkProgram()
{
    ctx()->LinkProgram(handle());
}

void Program::glUseProgram() const
{
    ctx()->UseProgram(handle());
}

void Program::glGetProgramiv(GLenum pname, GLint *params) const
{
    ctx()->GetProgramiv(handle(), pname, params);
}

void Program::glGetProgramInfoLog(GLsizei bufSize, GLsizei *length, GLchar *infoLog) const
{
    ctx()->GetProgramInfoLog(handle(), bufSize, length, infoLog);
}

GLint Program::glGetUniformBlockIndex(const GLchar *uniformBlockName) const
{
    return ctx()->GetUniformBlockIndex(handle(), uniformBlockName);
}

void Program::glUniformBlockBinding(GLuint uniformBlockIndex, GLuint uniformBlockBinding) const
{
    ctx()->UniformBlockBinding(handle(), uniformBlockIndex, uniformBlockBinding);
}

GLint Program::glGetProgramResourceIndex(GLenum programInterface, const GLchar *name) const
{
    return ctx()->GetProgramResourceIndex(handle(), programInterface, name);
}

void Program::glShaderStorageBlockBinding(GLuint storageBlockIndex, GLuint storageBlockBinding) const
{
    ctx()->ShaderStorageBlockBinding(handle(), storageBlockIndex, storageBlockBinding);
}

GLint Program::glGetAttribLocation(const GLchar* name) const
{
    return ctx()->GetAttribLocation(handle(), name);
}

GLint Program::glGetUniformLocation(const GLchar* name) const
{
    return ctx()->GetUniformLocation(handle(), name);
}

GLuint* Program::create_handle(const std::shared_ptr<GladGLContext> &ctx)
{
    GLuint* handle = new GLuint(0);
    *handle = ctx->CreateProgram();
    return handle;
}

void Program::handle_deleter(GLuint* handle, GladGLContext& ctx)
{
    ctx.DeleteProgram(*handle);
    delete handle;
}
