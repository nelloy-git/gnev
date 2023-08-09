#include "gl/GLBuffer.hpp"

#include <iostream>

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

void GLBuffer::glBufferData(GLsizeiptr size, const void *data, GLenum usage){
    if (ctx()->VERSION_4_5){
        ctx()->NamedBufferData(handle(), size, data, usage);
    } else {
        GLint prev_bind;
        ctx()->GetIntegerv(GL_ARRAY_BUFFER, &prev_bind);

        ctx()->BindBuffer(GL_ARRAY_BUFFER, handle());
        ctx()->BufferData(GL_ARRAY_BUFFER, size, data, usage);
        ctx()->BindBuffer(GL_ARRAY_BUFFER, prev_bind);
    }
}

void GLBuffer::glBufferStorage(GLsizeiptr size, const void* data, GLbitfield flags){
    if (ctx()->VERSION_4_5){
        ctx()->NamedBufferStorage(handle(), size, data, flags);
    } else {
        GLint prev_bind;
        ctx()->GetIntegerv(GL_ARRAY_BUFFER, &prev_bind);

        ctx()->BindBuffer(GL_ARRAY_BUFFER, handle());
        ctx()->BufferStorage(GL_ARRAY_BUFFER, size, data, flags);
        ctx()->BindBuffer(GL_ARRAY_BUFFER, prev_bind);
    }
}

void GLBuffer::glBufferSubData(GLintptr offset, GLsizeiptr size, const void *data){
    if (ctx()->VERSION_4_5){
        ctx()->NamedBufferSubData(handle(), offset, size, data);
    } else {
        GLint prev_bind;
        ctx()->GetIntegerv(GL_ARRAY_BUFFER, &prev_bind);

        ctx()->BindBuffer(GL_ARRAY_BUFFER, handle());
        ctx()->BufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        ctx()->BindBuffer(GL_ARRAY_BUFFER, prev_bind);
    }
}

void GLBuffer::glGetBufferSubData(GLintptr offset, GLsizeiptr size, void *data) const {
    if (ctx()->VERSION_4_5){
        ctx()->GetNamedBufferSubData(handle(), offset, size, data);
    } else {
        GLint prev_bind;
        ctx()->GetIntegerv(GL_ARRAY_BUFFER, &prev_bind);

        ctx()->BindBuffer(GL_ARRAY_BUFFER, handle());
        ctx()->GetBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        ctx()->BindBuffer(GL_ARRAY_BUFFER, prev_bind);
    }
}

void GLBuffer::glCopyBufferSubData(GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const {
    if (ctx()->VERSION_4_5){
        ctx()->CopyNamedBufferSubData(handle(), writeBuffer, readOffset, writeOffset, size);
    } else {
        GLint prev_read_bind, prev_write_bind;
        ctx()->GetIntegerv(GL_COPY_READ_BUFFER_BINDING, &prev_read_bind);
        ctx()->GetIntegerv(GL_COPY_WRITE_BUFFER_BINDING, &prev_write_bind);

        ctx()->BindBuffer(GL_COPY_READ_BUFFER, handle());
        ctx()->BindBuffer(GL_COPY_WRITE_BUFFER, writeBuffer);
        ctx()->CopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, readOffset, writeOffset, size);
        ctx()->BindBuffer(GL_COPY_READ_BUFFER, prev_read_bind);
        ctx()->BindBuffer(GL_COPY_WRITE_BUFFER, prev_write_bind);
    }
}

void GLBuffer::glGetBufferParameteriv(GLenum pname, GLint* params) const {
    ctx()->GetNamedBufferParameteriv(handle(), pname, params);
}

void* GLBuffer::glMapBuffer(GLenum access){
    void* ptr;
    if (ctx()->VERSION_4_5){
        ptr = ctx()->MapNamedBuffer(handle(), access);
    } else {
        GLint prev_bind;
        ctx()->GetIntegerv(GL_ARRAY_BUFFER, &prev_bind);

        ctx()->BindBuffer(GL_ARRAY_BUFFER, handle());
        ptr = ctx()->MapBuffer(GL_ARRAY_BUFFER, access);
        ctx()->BindBuffer(GL_ARRAY_BUFFER, prev_bind);
    }
    return ptr;
}

void* GLBuffer::glMapBufferRange(GLintptr offset, GLsizeiptr length, GLbitfield access){
    void* ptr;
    if (ctx()->VERSION_4_5){
        ptr = ctx()->MapNamedBufferRange(handle(), offset, length, access);
    } else {
        GLint prev_bind;
        ctx()->GetIntegerv(GL_ARRAY_BUFFER, &prev_bind);

        ctx()->BindBuffer(GL_ARRAY_BUFFER, handle());
        ptr = ctx()->MapNamedBufferRange(GL_ARRAY_BUFFER, offset, length, access);
        ctx()->BindBuffer(GL_ARRAY_BUFFER, prev_bind);
    }
    return ptr;
}

void GLBuffer::glFlushMappedBufferRange(GLintptr offset, GLsizeiptr length)
{
    ctx()->FlushMappedNamedBufferRange(handle(), offset, length);
}

void GLBuffer::glUnmapBuffer(){
    if (ctx()->VERSION_4_5){
        ctx()->UnmapNamedBuffer(handle());
    } else {
        GLint prev_bind;
        ctx()->GetIntegerv(GL_ARRAY_BUFFER, &prev_bind);

        ctx()->BindBuffer(GL_ARRAY_BUFFER, handle());
        ctx()->UnmapBuffer(GL_ARRAY_BUFFER);
        ctx()->BindBuffer(GL_ARRAY_BUFFER, prev_bind);
    }
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