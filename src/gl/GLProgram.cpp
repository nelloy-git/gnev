#include "gl/GLProgram.hpp"

using namespace gnev;

GLProgram::GLProgram(const std::shared_ptr<GladGLContext> &ctx) :
    handle(create_program(ctx)),
    ctx(ctx){
}

GLProgram::~GLProgram(){
    ctx->DeleteProgram(handle);
}

void GLProgram::glAttachShader(GLuint shader){
    ctx->AttachShader(handle, shader);
}

void GLProgram::glValidateProgram(){
    ctx->ValidateProgram(handle);
}

void GLProgram::glLinkProgram(){
    ctx->LinkProgram(handle);
}

void GLProgram::glUseProgram(){
    ctx->UseProgram(handle);
}

void GLProgram::glGetProgramiv(GLenum pname, GLint *params) const {
    ctx->GetProgramiv(handle, pname, params);
}

void GLProgram::glGetProgramInfoLog(GLsizei bufSize, GLsizei *length, GLchar *infoLog) const {
    ctx->GetProgramInfoLog(handle, bufSize, length, infoLog);
}

GLuint GLProgram::glGetUniformBlockIndex(const GLchar *uniformBlockName) const {
    return ctx->GetUniformBlockIndex(handle, uniformBlockName);
}

GLint GLProgram::glGetAttribLocation(const GLchar* name) const {
    return ctx->GetAttribLocation(handle, name);
}

std::pair<bool, std::string> GLProgram::validate_with_info_log(){
    glValidateProgram();

    GLint status;
    glGetProgramiv(GL_VALIDATE_STATUS, &status);

    GLint len;
    glGetProgramiv(GL_INFO_LOG_LENGTH, &len);

    std::string dst;
    dst.resize(len);
    glGetProgramInfoLog(len, &len, dst.data());
    return {status, dst};
}

std::pair<bool, std::string> GLProgram::link_with_info_log(){
    glLinkProgram();

    GLint status;
    glGetProgramiv(GL_LINK_STATUS, &status);

    GLint len;
    glGetProgramiv(GL_INFO_LOG_LENGTH, &len);

    std::string dst;
    dst.resize(len);
    glGetProgramInfoLog(len, &len, dst.data());
    return {status, dst};
}

GLuint GLProgram::create_program(const std::shared_ptr<GladGLContext> &ctx){
    return ctx->CreateProgram();
}
