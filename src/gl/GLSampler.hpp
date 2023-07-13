#pragma once

#include <memory>

#include "glad/gl.h"

#include "util/Util.hpp"

namespace gnev {

class EXPORT GLSampler final {
public:
    GLSampler(const std::shared_ptr<GladGLContext> &ctx);
    ~GLSampler();

    void glBindSampler(GLuint unit);
    void glSamplerParameteri(GLuint pname, GLint param);
    void glSamplerParameterf(GLuint pname, GLfloat param);
    void glSamplerParameterfv(GLuint pname, const GLfloat* param);

    const std::shared_ptr<GladGLContext> ctx;
    const GLuint handle;

private:
    static GLuint create_sampler(const std::shared_ptr<GladGLContext> &ctx);
    
};

}