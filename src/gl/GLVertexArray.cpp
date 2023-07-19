#include "gl/GLVertexArray.hpp"

using namespace gnev;

GLVertexArray::GLVertexArray(const std::shared_ptr<GladGLContext> &ctx) :
    GLHandler(ctx, create_handle(ctx), &handle_deleter){
}

GLVertexArray::~GLVertexArray(){
}

void GLVertexArray::glBindVertexArray() const {
    ctx()->BindVertexArray(handle());
}

void GLVertexArray::glVertexArrayElementBuffer(GLuint buffer){
    ctx()->VertexArrayElementBuffer(handle(), buffer);
}

void GLVertexArray::glVertexArrayVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride){
    ctx()->VertexArrayVertexBuffer(handle(), bindingindex, buffer, offset, stride);
}

void GLVertexArray::glVertexArrayAttribBinding(GLuint attribindex, GLuint bindingindex){
    ctx()->VertexArrayAttribBinding(handle(), attribindex, bindingindex);
}

void GLVertexArray::glVertexArrayAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset){
    ctx()->VertexArrayAttribFormat(handle(), attribindex, size, type, normalized, relativeoffset);
}

void GLVertexArray::glVertexArrayBindingDivisor(GLuint bindingindex, GLuint divisor){
    ctx()->VertexArrayBindingDivisor(handle(), bindingindex, divisor);
}

void GLVertexArray::glEnableVertexArrayAttrib(GLuint index){
    ctx()->EnableVertexArrayAttrib(handle(), index);
}

void GLVertexArray::glDisableVertexArrayAttrib(GLuint index){
    ctx()->DisableVertexArrayAttrib(handle(), index);
}


GLuint* GLVertexArray::create_handle(const std::shared_ptr<GladGLContext> &ctx){
    GLuint* handle = new GLuint(0);
    if (ctx->VERSION_4_5){
        ctx->CreateVertexArrays(1, handle);
    } else {
        ctx->GenVertexArrays(1, handle);
    }
    return handle;
}

void GLVertexArray::handle_deleter(GLuint* handle, GladGLContext& ctx){
    ctx.DeleteVertexArrays(1, handle);
    delete handle;
}