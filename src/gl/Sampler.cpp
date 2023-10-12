#include "gl/Sampler.hpp"

using namespace gnev::gl;

Sampler::Sampler()
    : Handler(createHandle(), &deleteHandle) {}

Sampler::~Sampler() {}

void Sampler::glBindSampler(GLuint unit) { Ctx::Get().glBindSampler(unit, handle()); }

void Sampler::glSamplerParameteri(GLuint pname, GLint param) {
    Ctx::Get().glSamplerParameteri(handle(), pname, param);
}

void Sampler::glSamplerParameterf(GLuint pname, GLfloat param) {
    Ctx::Get().glSamplerParameterf(handle(), pname, param);
}

void Sampler::glSamplerParameterfv(GLuint pname, const GLfloat* param) {
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