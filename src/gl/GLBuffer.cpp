#include "gl/GLBuffer.hpp"

using namespace gnev;

GLBuffer::GLBuffer(const std::shared_ptr<GladGLContext> &ctx) :
    GLHandler(ctx, create_handle(ctx), &handle_deleter){
}

GLBuffer::~GLBuffer(){
}

void GLBuffer::glBindBuffer(GLenum target) const {
    ctx()->BindBuffer(target, handle());
}

void GLBuffer::glBindBufferBase(GLenum target, GLuint index) const {
    ctx()->BindBufferBase(target, index, handle());
}

void GLBuffer::glBindBufferRange(GLenum target, GLuint index, GLintptr offset, GLsizeiptr size) const {
    ctx()->BindBufferRange(target, index, handle(), offset, size);
}

void GLBuffer::glBufferStorage(GLsizeiptr size, const void* data, GLbitfield flags){
    if (ctx()->VERSION_4_5){
        ctx()->NamedBufferStorage(handle(), size, data, flags);
    } else {
        ctx()->BindBuffer(GL_ARRAY_BUFFER, handle());
        ctx()->BufferStorage(GL_ARRAY_BUFFER, size, data, flags);
        ctx()->BindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void GLBuffer::glBufferSubData(GLintptr offset, GLsizeiptr size, const void *data){
    if (ctx()->VERSION_4_5){
        ctx()->NamedBufferSubData(handle(), offset, size, data);
    } else {
        ctx()->BindBuffer(GL_ARRAY_BUFFER, handle());
        ctx()->BufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        ctx()->BindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void GLBuffer::glGetBufferSubData(GLintptr offset, GLsizeiptr size, void *data) const {
    if (ctx()->VERSION_4_5){
        ctx()->GetNamedBufferSubData(handle(), offset, size, data);
    } else {
        ctx()->BindBuffer(GL_READ_BUFFER, handle());
        ctx()->GetBufferSubData(GL_READ_BUFFER, offset, size, data);
        ctx()->BindBuffer(GL_READ_BUFFER, 0);
    }
}

void GLBuffer::glCopyBufferSubData(GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const {
    if (ctx()->VERSION_4_5){
        ctx()->CopyNamedBufferSubData(handle(), writeBuffer, readOffset, writeOffset, size);
    } else {
        ctx()->BindBuffer(GL_COPY_READ_BUFFER, handle());
        ctx()->BindBuffer(GL_COPY_WRITE_BUFFER, writeBuffer);
        ctx()->CopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, readOffset, writeOffset, size);
        ctx()->BindBuffer(GL_COPY_READ_BUFFER, 0);
        ctx()->BindBuffer(GL_COPY_WRITE_BUFFER, 0);
    }
}

void GLBuffer::glGetBufferParameteriv(GLenum pname, GLint* params) const {
    ctx()->GetNamedBufferParameteriv(handle(), pname, params);
}

GLuint* GLBuffer::create_handle(const std::shared_ptr<GladGLContext> &ctx){
    GLuint* handle = new GLuint(0);
    if (ctx->VERSION_4_5){
        ctx->CreateBuffers(1, handle);
    } else {
        ctx->GenBuffers(1, handle);
    }
    return handle;
}

void GLBuffer::handle_deleter(GLuint* handle, GladGLContext& ctx){
    ctx.DeleteBuffers(1, handle);
    delete handle;
}