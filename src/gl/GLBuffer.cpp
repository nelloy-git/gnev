#include "gl/GLBuffer.hpp"

using namespace gnev;

GLBuffer::GLBuffer(const std::shared_ptr<GladGLContext> &ctx) :
    handle(create_buffer(ctx)),
    ctx(ctx){
}

GLBuffer::~GLBuffer(){
    ctx->DeleteBuffers(1, &handle);
}

void GLBuffer::glBindBuffer(GLenum target) const {
    ctx->BindBuffer(target, handle);
}

void GLBuffer::glBufferStorage(GLsizeiptr size, const void* data, GLbitfield flags){
    if (ctx->VERSION_4_5){
        ctx->NamedBufferStorage(handle, size, data, flags);
    } else {
        ctx->BindBuffer(GL_ARRAY_BUFFER, handle);
        ctx->BufferStorage(GL_ARRAY_BUFFER, size, data, flags);
        ctx->BindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void GLBuffer::glBufferSubData(GLintptr offset, GLsizeiptr size, const void *data){
    if (ctx->VERSION_4_5){
        ctx->NamedBufferSubData(handle, offset, size, data);
    } else {
        ctx->BindBuffer(GL_ARRAY_BUFFER, handle);
        ctx->BufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        ctx->BindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void GLBuffer::glGetBufferSubData(GLintptr offset, GLsizeiptr size, void *data) const {
    if (ctx->VERSION_4_5){
        ctx->GetNamedBufferSubData(handle, offset, size, data);
    } else {
        ctx->BindBuffer(GL_READ_BUFFER, handle);
        ctx->GetBufferSubData(GL_READ_BUFFER, offset, size, data);
        ctx->BindBuffer(GL_READ_BUFFER, 0);
    }
}

void GLBuffer::glCopyBufferSubData(GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const {
    if (ctx->VERSION_4_5){
        ctx->CopyNamedBufferSubData(handle, writeBuffer, readOffset, writeOffset, size);
    } else {
        ctx->BindBuffer(GL_COPY_READ_BUFFER, handle);
        ctx->BindBuffer(GL_COPY_WRITE_BUFFER, writeBuffer);
        ctx->CopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, readOffset, writeOffset, size);
        ctx->BindBuffer(GL_COPY_READ_BUFFER, 0);
        ctx->BindBuffer(GL_COPY_WRITE_BUFFER, 0);
    }
}

GLuint GLBuffer::create_buffer(const std::shared_ptr<GladGLContext> &ctx){
    GLuint handle;
    if (ctx->VERSION_4_5){
        ctx->CreateBuffers(1, &handle);
    } else {
        ctx->GenBuffers(1, &handle);
    }
    return handle;
}
