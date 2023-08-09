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

class EXPORT GLBufferVectorCoherent : public GLBuffer {
public:
    using size_type = unsigned int;

    static constexpr size_type CAP_BASE = GNEV_GL_BUFFER_VECTOR_CAP_BASE;
    static constexpr size_type CAP_MULT = GNEV_GL_BUFFER_VECTOR_CAP_MULT;
    static constexpr size_type CAP_ADD = GNEV_GL_BUFFER_VECTOR_CAP_ADD;

    GLBufferVectorCoherent(const std::shared_ptr<GladGLContext> &ctx, const void* initial_data, size_type initial_size, GLenum usage);
    virtual ~GLBufferVectorCoherent();

    void* at(size_type offset);
    const void* at(size_type offset) const;
    void* data();
    const void* data() const;

    bool empty() const;
    size_type size() const;
    void reserve(size_type capacity);
    size_type capacity() const;
    void shrink_to_fit();

    void clear();
    void insert(size_type offset, size_type size, const void* data);
    void erase(size_type offset, size_type size);
    void push_back(size_type size, const void* data);
    void pop_back(size_type size);

private:
    static const GLbitfield _map_access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

    size_type _size;
    size_type _cap;
    std::shared_ptr<unsigned char*> _map;
};


}