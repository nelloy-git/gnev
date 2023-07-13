#include "gl/GLShader.hpp"

using namespace gnev;

GLShader::GLShader(const std::shared_ptr<GladGLContext> &ctx, GLenum type) :
    handle(create_shader(ctx, type)),
    ctx(ctx){
}

GLShader::~GLShader(){
    ctx->DeleteShader(handle);
}

void GLShader::glShaderSource(GLsizei count, const GLchar *const *string, const GLint *length){
    ctx->ShaderSource(handle, count, string, length);   
}

void GLShader::glCompileShader(){
    ctx->CompileShader(handle);   
}

void GLShader::glGetShaderiv(GLenum pname, GLint* params) const {
    ctx->GetShaderiv(handle, pname, params);
}

void GLShader::glGetShaderInfoLog(GLsizei bufSize, GLsizei *length, GLchar *infoLog) const {
    ctx->GetShaderInfoLog(handle, bufSize, length, infoLog);
}

std::pair<bool, std::string> GLShader::compile_with_info_log(const std::string& src){
    auto c_str = src.c_str();
    auto str_len = static_cast<GLint>(src.size());
    glShaderSource(1, &c_str, &str_len);

    glCompileShader();
    GLint status;
    glGetShaderiv(GL_COMPILE_STATUS, &status);

    GLint len;
    glGetShaderiv(GL_INFO_LOG_LENGTH, &len);

    std::string dst;
    dst.resize(len);
    glGetShaderInfoLog(len, &len, dst.data());

    return {status == GL_TRUE, dst};
}

GLuint GLShader::create_shader(const std::shared_ptr<GladGLContext> &ctx, GLenum type){
    return ctx->CreateShader(type);
}
