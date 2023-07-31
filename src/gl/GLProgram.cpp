#include "gl/GLProgram.hpp"

using namespace gnev;

GLProgram::GLProgram(const std::shared_ptr<GladGLContext> &ctx) :
    GLHandler(ctx, create_handle(ctx), &handle_deleter)
{
}

GLProgram::~GLProgram()
{
}

void GLProgram::glAttachShader(GLuint shader)
{
    ctx()->AttachShader(handle(), shader);
}

void GLProgram::glValidateProgram()
{
    ctx()->ValidateProgram(handle());
}

void GLProgram::glLinkProgram()
{
    ctx()->LinkProgram(handle());
}

void GLProgram::glUseProgram() const
{
    ctx()->UseProgram(handle());
}

void GLProgram::glGetProgramiv(GLenum pname, GLint *params) const
{
    ctx()->GetProgramiv(handle(), pname, params);
}

void GLProgram::glGetProgramInfoLog(GLsizei bufSize, GLsizei *length, GLchar *infoLog) const
{
    ctx()->GetProgramInfoLog(handle(), bufSize, length, infoLog);
}

GLint GLProgram::glGetUniformBlockIndex(const GLchar *uniformBlockName) const
{
    return ctx()->GetUniformBlockIndex(handle(), uniformBlockName);
}

void GLProgram::glUniformBlockBinding(GLuint uniformBlockIndex, GLuint uniformBlockBinding) const
{
    ctx()->UniformBlockBinding(handle(), uniformBlockIndex, uniformBlockBinding);
}

GLint GLProgram::glGetProgramResourceIndex(GLenum programInterface, const GLchar *name) const
{
    return ctx()->GetProgramResourceIndex(handle(), programInterface, name);
}

void GLProgram::glShaderStorageBlockBinding(GLuint storageBlockIndex, GLuint storageBlockBinding) const
{
    ctx()->ShaderStorageBlockBinding(handle(), storageBlockIndex, storageBlockBinding);
}

GLint GLProgram::glGetAttribLocation(const GLchar* name) const
{
    return ctx()->GetAttribLocation(handle(), name);
}

GLint GLProgram::glGetUniformLocation(const GLchar* name) const
{
    return ctx()->GetUniformLocation(handle(), name);
}

GLuint* GLProgram::create_handle(const std::shared_ptr<GladGLContext> &ctx)
{
    GLuint* handle = new GLuint(0);
    *handle = ctx->CreateProgram();
    return handle;
}

void GLProgram::handle_deleter(GLuint* handle, GladGLContext& ctx)
{
    ctx.DeleteProgram(*handle);
    delete handle;
}
