#pragma once

#include "gl/GLHandler.hpp"

namespace gnev {

class EXPORT GLBuffer : public GLHandler {
public:
    GLBuffer(const std::shared_ptr<GladGLContext> &ctx);
    virtual ~GLBuffer();

    void glBindBuffer(GLenum target) const;
    void glBindBufferBase(GLenum target, GLuint index) const;
    void glBindBufferRange(GLenum target, GLuint index, GLintptr offset, GLsizeiptr size) const;
    void glBufferData(GLsizeiptr size, const void *data, GLenum usage);
    void glBufferStorage(GLsizeiptr size, const void* data, GLbitfield flags);
    void glBufferSubData(GLintptr offset, GLsizeiptr size, const void *data);
    void glGetBufferSubData(GLintptr offset, GLsizeiptr size, void *data) const;
    void glCopyBufferSubData(GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const;
    void glGetBufferParameteriv(GLenum pname, GLint* params) const;
    void* glMapBuffer(GLenum access);
    void* glMapBufferRange(GLintptr offset, GLsizeiptr length, GLbitfield access);
    void glFlushMappedBufferRange(GLintptr offset, GLsizeiptr length);
    void glUnmapBuffer();

private:
    static GLuint* create_handle(const std::shared_ptr<GladGLContext> &ctx);
    static void handle_deleter(GLuint* handle, GladGLContext& ctx);

};

}