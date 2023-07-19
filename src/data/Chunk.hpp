#pragma once

#include "gl/GLBufferContinuousMap.hpp"
#include "gl/GLVertexArray.hpp"

namespace gnev {

class Chunk {
public:
    Chunk(const std::shared_ptr<GladGLContext>& ctx);
    virtual ~Chunk();

private:
    GLVertexArray _vao;
    GLBufferContinuousMap<GLuint> _ibo;
    GLBufferContinuousMap<GLuint> _vbo;

};

}