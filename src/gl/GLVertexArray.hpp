#pragma once

#include <memory>

#include "glad/gl.h"

#include "util/Util.hpp"

namespace gnev {

class EXPORT GLVertexArray final {
public:
    GLVertexArray(const std::shared_ptr<GladGLContext> &ctx);
    GLVertexArray(const GLVertexArray&) = delete;
    GLVertexArray(GLVertexArray&&) = delete;
    ~GLVertexArray();

    void glBindVertexArray() const;

    const std::shared_ptr<GladGLContext> ctx;
    const GLuint handle;

private:
    static GLuint create_vertex_array(const std::shared_ptr<GladGLContext> &ctx);
};

}