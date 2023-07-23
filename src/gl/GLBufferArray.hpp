#pragma once

#include <stdexcept>

#include "gl/GLBuffer.hpp"

namespace gnev {

class EXPORT GLBufferArray : public GLBuffer {
public:
    GLBufferArray(const std::shared_ptr<GladGLContext> &ctx, const void* initial_data, GLsizeiptr initial_size, GLbitfield storage_flags);
    ~GLBufferArray();

    GLsizeiptr size() const;

    std::shared_ptr<void> get(GLintptr offset, GLsizeiptr size) const;
    void set(GLintptr offset, GLsizeiptr size, const void* data);

private:
    const GLuint _size;
};

}