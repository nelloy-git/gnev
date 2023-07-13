#pragma once

#include <memory>

#include "glad/gl.h"

#include "util/Util.hpp"

namespace gnev {

class EXPORT GLBuffer final {
public:
    GLBuffer(const std::shared_ptr<GladGLContext> &ctx);
    GLBuffer(const GLBuffer&) = delete;
    GLBuffer(GLBuffer&&) = delete;
    ~GLBuffer();

    void glBindBuffer(GLenum target) const;
    void glBufferStorage(GLsizeiptr size, const void* data, GLbitfield flags);
    void glBufferSubData(GLintptr offset, GLsizeiptr size, const void *data);
    void glGetBufferSubData(GLintptr offset, GLsizeiptr size, void *data) const;
    void glCopyBufferSubData(GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) const;

    const std::shared_ptr<GladGLContext> ctx;
    const GLuint handle;

private:
    static GLuint create_buffer(const std::shared_ptr<GladGLContext> &ctx);

};

}