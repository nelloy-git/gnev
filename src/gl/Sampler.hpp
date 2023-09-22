#pragma once

#include "gl/Handler.hpp"

namespace gnev::gl {

class EXPORT Sampler : public Handler {
public:
    Sampler(const std::shared_ptr<GladGLContext>& ctx);
    virtual ~Sampler();

    void glBindSampler(GLuint unit);
    void glSamplerParameteri(GLuint pname, GLint param);
    void glSamplerParameterf(GLuint pname, GLfloat param);
    void glSamplerParameterfv(GLuint pname, const GLfloat* param);

private:
    static GLuint* create_handle(const std::shared_ptr<GladGLContext>& ctx);
    static void handle_deleter(GLuint* handle, GladGLContext& ctx);
};

} // namespace gnev::gl