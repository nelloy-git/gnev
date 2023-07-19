#include "gl/GLShader.hpp"

using namespace gnev;

GLShader::GLShader(const std::shared_ptr<GladGLContext> &ctx, GLenum type) :
    GLHandler(ctx, create_handle(ctx, type), &handle_deleter)
{
}

GLShader::~GLShader(){
    ctx()->DeleteShader(handle());
}

void GLShader::glShaderSource(GLsizei count, const GLchar *const *string, const GLint *length){
    ctx()->ShaderSource(handle(), count, string, length);   
}

void GLShader::glCompileShader(){
    ctx()->CompileShader(handle());   
}

void GLShader::glGetShaderiv(GLenum pname, GLint* params) const {
    ctx()->GetShaderiv(handle(), pname, params);
}

void GLShader::glGetShaderInfoLog(GLsizei bufSize, GLsizei *length, GLchar *infoLog) const {
    ctx()->GetShaderInfoLog(handle(), bufSize, length, infoLog);
}

GLuint* GLShader::create_handle(const std::shared_ptr<GladGLContext> &ctx, GLenum type){
    GLuint* handle = new GLuint(0);
    *handle = ctx->CreateShader(type);
    return handle;
}

void GLShader::handle_deleter(GLuint* handle, GladGLContext& ctx){
    ctx.DeleteShader(*handle);
    delete handle;
}
