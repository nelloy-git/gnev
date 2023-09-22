#include "gl/Buffer.hpp"

#include <iostream>

using namespace gnev::gl;

Buffer::Buffer(const std::shared_ptr<GladGLContext>& ctx)
    : Handler(ctx, create_handle(ctx), &handle_deleter) {}

Buffer::~Buffer() {}

void Buffer::glBindBuffer(GLenum target) const { ctx()->BindBuffer(target, handle()); }

void Buffer::glBindBufferBase(GLenum target, GLuint index) const {
    ctx()->BindBufferBase(target, index, handle());
}

void Buffer::glBindBufferRange(GLenum target,
                               GLuint index,
                               GLintptr offset,
                               GLsizeiptr size) const {
    ctx()->BindBufferRange(target, index, handle(), offset, size);
}

void Buffer::glBufferData(GLsizeiptr size, const void* data, GLenum usage) {
    if (ctx()->VERSION_4_5) {
        ctx()->NamedBufferData(handle(), size, data, usage);
    } else {
        GLint prev_bind;
        ctx()->GetIntegerv(GL_ARRAY_BUFFER, &prev_bind);

        ctx()->BindBuffer(GL_ARRAY_BUFFER, handle());
        ctx()->BufferData(GL_ARRAY_BUFFER, size, data, usage);
        ctx()->BindBuffer(GL_ARRAY_BUFFER, prev_bind);
    }
}

void Buffer::glBufferStorage(GLsizeiptr size, const void* data, GLbitfield flags) {
    if (ctx()->VERSION_4_5) {
        ctx()->NamedBufferStorage(handle(), size, data, flags);
    } else {
        GLint prev_bind;
        ctx()->GetIntegerv(GL_ARRAY_BUFFER, &prev_bind);

        ctx()->BindBuffer(GL_ARRAY_BUFFER, handle());
        ctx()->BufferStorage(GL_ARRAY_BUFFER, size, data, flags);
        ctx()->BindBuffer(GL_ARRAY_BUFFER, prev_bind);
    }
}

void Buffer::glBufferSubData(GLintptr offset, GLsizeiptr size, const void* data) {
    if (ctx()->VERSION_4_5) {
        ctx()->NamedBufferSubData(handle(), offset, size, data);
    } else {
        GLint prev_bind;
        ctx()->GetIntegerv(GL_ARRAY_BUFFER, &prev_bind);

        ctx()->BindBuffer(GL_ARRAY_BUFFER, handle());
        ctx()->BufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        ctx()->BindBuffer(GL_ARRAY_BUFFER, prev_bind);
    }
}

void Buffer::glGetBufferSubData(GLintptr offset, GLsizeiptr size, void* data) const {
    if (ctx()->VERSION_4_5) {
        ctx()->GetNamedBufferSubData(handle(), offset, size, data);
    } else {
        GLint prev_bind;
        ctx()->GetIntegerv(GL_ARRAY_BUFFER, &prev_bind);

        ctx()->BindBuffer(GL_ARRAY_BUFFER, handle());
        ctx()->GetBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        ctx()->BindBuffer(GL_ARRAY_BUFFER, prev_bind);
    }
}

void Buffer::glCopyBufferSubData(GLuint writeBuffer,
                                 GLintptr readOffset,
                                 GLintptr writeOffset,
                                 GLsizeiptr size) const {
    if (ctx()->VERSION_4_5) {
        ctx()->CopyNamedBufferSubData(handle(), writeBuffer, readOffset, writeOffset, size);
    } else {
        GLint prev_read_bind, prev_write_bind;
        ctx()->GetIntegerv(GL_COPY_READ_BUFFER_BINDING, &prev_read_bind);
        ctx()->GetIntegerv(GL_COPY_WRITE_BUFFER_BINDING, &prev_write_bind);

        ctx()->BindBuffer(GL_COPY_READ_BUFFER, handle());
        ctx()->BindBuffer(GL_COPY_WRITE_BUFFER, writeBuffer);
        ctx()->CopyBufferSubData(GL_COPY_READ_BUFFER,
                                 GL_COPY_WRITE_BUFFER,
                                 readOffset,
                                 writeOffset,
                                 size);
        ctx()->BindBuffer(GL_COPY_READ_BUFFER, prev_read_bind);
        ctx()->BindBuffer(GL_COPY_WRITE_BUFFER, prev_write_bind);
    }
}

void Buffer::glGetBufferParameteriv(GLenum pname, GLint* params) const {
    ctx()->GetNamedBufferParameteriv(handle(), pname, params);
}

void* Buffer::glMapBuffer(GLenum access) {
    void* ptr;
    if (ctx()->VERSION_4_5) {
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

void* Buffer::glMapBufferRange(GLintptr offset, GLsizeiptr length, GLbitfield access) {
    void* ptr;
    if (ctx()->VERSION_4_5) {
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

void Buffer::glFlushMappedBufferRange(GLintptr offset, GLsizeiptr length) {
    ctx()->FlushMappedNamedBufferRange(handle(), offset, length);
}

void Buffer::glUnmapBuffer() {
    if (ctx()->VERSION_4_5) {
        ctx()->UnmapNamedBuffer(handle());
    } else {
        GLint prev_bind;
        ctx()->GetIntegerv(GL_ARRAY_BUFFER, &prev_bind);

        ctx()->BindBuffer(GL_ARRAY_BUFFER, handle());
        ctx()->UnmapBuffer(GL_ARRAY_BUFFER);
        ctx()->BindBuffer(GL_ARRAY_BUFFER, prev_bind);
    }
}

GLuint* Buffer::create_handle(const std::shared_ptr<GladGLContext>& ctx) {
    GLuint* handle = new GLuint(0);
    if (ctx->VERSION_4_5) {
        ctx->CreateBuffers(1, handle);
    } else {
        ctx->GenBuffers(1, handle);
    }
    return handle;
}

void Buffer::handle_deleter(GLuint* handle, GladGLContext& ctx) {
    ctx.DeleteBuffers(1, handle);
    delete handle;
}