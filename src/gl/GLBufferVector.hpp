#pragma once

#include <stdexcept>

#include "gl/GLBuffer.hpp"

#ifndef GNEV_GL_BUFFER_VECTOR_CAP_BASE
#define GNEV_GL_BUFFER_VECTOR_CAP_BASE 1
#endif

#ifndef GNEV_GL_BUFFER_VECTOR_CAP_MULT
#define GNEV_GL_BUFFER_VECTOR_CAP_MULT 2
#endif

#ifndef GNEV_GL_BUFFER_VECTOR_CAP_ADD
#define GNEV_GL_BUFFER_VECTOR_CAP_ADD 0
#endif

namespace gnev {

class EXPORT GLBufferVector : public GLBuffer {
public:
    static constexpr GLsizeiptr CAP_BASE = GNEV_GL_BUFFER_VECTOR_CAP_BASE;
    static constexpr GLsizeiptr CAP_MULT = GNEV_GL_BUFFER_VECTOR_CAP_MULT;
    static constexpr GLsizeiptr CAP_ADD = GNEV_GL_BUFFER_VECTOR_CAP_ADD;

    GLBufferVector(const std::shared_ptr<GladGLContext> &ctx, const void* initial_data, GLsizeiptr initial_size, GLenum usage);
    virtual ~GLBufferVector();

    GLsizeiptr size() const;
    GLsizeiptr capacity() const;

    std::shared_ptr<void> get(GLintptr offset, GLsizeiptr size) const;
    void set(GLintptr offset, GLsizeiptr size, const void* data);
    void push_back(GLsizeiptr size, const void* data);
    void insert(GLintptr offset, GLsizeiptr size, const void* data);
    void remove(GLintptr offset, GLsizeiptr size);
    void reserve(GLsizeiptr capacity);
    void shrink_to_fit();

private:
    GLsizeiptr _size;
    GLsizeiptr _cap;

};


}