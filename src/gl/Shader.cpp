#include "gl/Shader.hpp"

using namespace gnev::gl;

Shader::Shader(const std::shared_ptr<GladGLContext> &ctx, GLenum type) :
    Handler(ctx, create_handle(ctx, type), &handle_deleter)
{
}

Shader::~Shader(){
    ctx()->DeleteShader(handle());
}

void Shader::glShaderSource(GLsizei count, const GLchar *const *string, const GLint *length){
    ctx()->ShaderSource(handle(), count, string, length);   
}

void Shader::glCompileShader(){
    ctx()->CompileShader(handle());   
}

void Shader::glGetShaderiv(GLenum pname, GLint* params) const {
    ctx()->GetShaderiv(handle(), pname, params);
}

void Shader::glGetShaderInfoLog(GLsizei bufSize, GLsizei *length, GLchar *infoLog) const {
    ctx()->GetShaderInfoLog(handle(), bufSize, length, infoLog);
}

GLuint* Shader::create_handle(const std::shared_ptr<GladGLContext> &ctx, GLenum type){
    GLuint* handle = new GLuint(0);
    *handle = ctx->CreateShader(type);
    return handle;
}

void Shader::handle_deleter(GLuint* handle, GladGLContext& ctx){
    ctx.DeleteShader(*handle);
    delete handle;
}
