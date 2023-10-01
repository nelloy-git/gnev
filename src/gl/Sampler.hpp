#pragma once

#include "gl/Handler.hpp"

namespace gnev::gl {

class EXPORT Sampler : public Handler {
public:
    Sampler(const Ctx& ctx);
    virtual ~Sampler();

    void glBindSampler(GLuint unit);
    void glSamplerParameteri(GLuint pname, GLint param);
    void glSamplerParameterf(GLuint pname, GLfloat param);
    void glSamplerParameterfv(GLuint pname, const GLfloat* param);

private:
    static GLuint createHandle(const Ctx& ctx);
    static void freeHandle(const Ctx& ctx, GLuint handle);
};

} // namespace gnev::gl