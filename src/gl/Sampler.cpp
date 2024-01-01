#include "gl/Sampler.hpp"

namespace gnev::gl {

Ref<Sampler> Sampler::MakeNearestRepeat() {
    auto sampler = MakeSharable<Sampler>();
    sampler->setParam(GL_TEXTURE_WRAP_S, GL_REPEAT);
    sampler->setParam(GL_TEXTURE_WRAP_T, GL_REPEAT);
    sampler->setParam(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    sampler->setParam(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return sampler;
}

Sampler::Sampler()
    : Handler(createHandle(), &deleteHandle) {}

Sampler::~Sampler() {}

void Sampler::bind(GLuint unit) { Ctx::Get().glBindSampler(unit, handle()); }

void Sampler::setParam(GLuint pname, GLint param) {
    Ctx::Get().glSamplerParameteri(handle(), pname, param);
}

void Sampler::setParam(GLuint pname, GLfloat param) {
    Ctx::Get().glSamplerParameterf(handle(), pname, param);
}

void Sampler::setParam(GLuint pname, const GLfloat* param) {
    Ctx::Get().glSamplerParameterfv(handle(), pname, param);
}

GLuint* Sampler::createHandle() {
    auto handle = new GLuint(0);
    Ctx::Get().glCreateSamplers(1, handle);
    return handle;
}

void Sampler::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteSamplers(1, handle);
    delete handle;
}

} // namespace gnev::gl