#include "gl/Sampler.hpp"

using namespace gnev::gl;

Sampler::Sampler(const Ctx& ctx)
    : Handler(ctx, createHandle(ctx), &freeHandle) {}

Sampler::~Sampler() {}

void Sampler::glBindSampler(GLuint unit) { ctx().glBindSampler(unit, handle()); }

void Sampler::glSamplerParameteri(GLuint pname, GLint param) {
    ctx().glSamplerParameteri(handle(), pname, param);
}

void Sampler::glSamplerParameterf(GLuint pname, GLfloat param) {
    ctx().glSamplerParameterf(handle(), pname, param);
}

void Sampler::glSamplerParameterfv(GLuint pname, const GLfloat* param) {
    ctx().glSamplerParameterfv(handle(), pname, param);
}

GLuint Sampler::createHandle(const Ctx& ctx) {
    GLuint handle;
    ctx.glCreateSamplers(1, &handle);
    return handle;
}

void Sampler::freeHandle(const Ctx& ctx, GLuint handle) {
    ctx.glDeleteSamplers(1, &handle);
}