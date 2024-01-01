#pragma once

#include "gl/Handler.hpp"
#include "util/Ref.hpp"

namespace gnev::gl {

class EXPORT Sampler : public Handler {
public:
    static Ref<Sampler> MakeNearestRepeat();

    Sampler();
    Sampler(const Sampler&) = delete;
    Sampler(Sampler&&) = delete;
    virtual ~Sampler();

    void bind(GLuint unit);
    void setParam(GLuint pname, GLint param);
    void setParam(GLuint pname, GLfloat param);
    void setParam(GLuint pname, const GLfloat* param);

private:
    static GLuint* createHandle();
    static void deleteHandle(GLuint* handle);
};

} // namespace gnev::gl