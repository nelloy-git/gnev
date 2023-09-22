#pragma once

#include "gl/Handler.hpp"

namespace gnev::gl {

class EXPORT Buffer : public Handler {
public:
    Buffer(const GladCtx& ctx);
    virtual ~Buffer();

    void glBindBuffer(GLenum target) const;
    void glBindBufferBase(GLenum target, GLuint index) const;
    void glBindBufferRange(GLenum target, GLuint index, GLintptr offset, GLsizeiptr size) const;
    void glBufferData(GLsizeiptr size, const void* data, GLenum usage);
    void glBufferStorage(GLsizeiptr size, const void* data, GLbitfield flags);
    void glBufferSubData(GLintptr offset, GLsizeiptr size, const void* data);
    void glGetBufferSubData(GLintptr offset, GLsizeiptr size, void* data) const;
    void glCopyBufferSubData(GLuint writeBuffer,
                             GLintptr readOffset,
                             GLintptr writeOffset,
                             GLsizeiptr size) const;
    void glGetBufferParameteriv(GLenum pname, GLint* params) const;
    void* glMapBuffer(GLenum access);
    void* glMapBufferRange(GLintptr offset, GLsizeiptr length, GLbitfield access);
    void glFlushMappedBufferRange(GLintptr offset, GLsizeiptr length);
    void glUnmapBuffer();

private:
    static GLuint* create_handle(const GladCtx& ctx);
    static void handle_deleter(GLuint* handle, GladGLContext& ctx);
};

} // namespace gnev::gl