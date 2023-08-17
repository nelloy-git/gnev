#include "gl/Sampler.hpp"

using namespace gnev::gl;

Sampler::Sampler(const std::shared_ptr<GladGLContext> &ctx) :
    Handler(ctx, create_handle(ctx), &handle_deleter)
{
}

Sampler::~Sampler()
{
}

void Sampler::glBindSampler(GLuint unit)
{
    ctx()->BindSampler(unit, handle());
}

void Sampler::glSamplerParameteri(GLuint pname, GLint param)
{
    ctx()->SamplerParameteri(handle(), pname, param);
}

void Sampler::glSamplerParameterf(GLuint pname, GLfloat param)
{
    ctx()->SamplerParameterf(handle(), pname, param);
}

void Sampler::glSamplerParameterfv(GLuint pname, const GLfloat* param)
{
    ctx()->SamplerParameterfv(handle(), pname, param);
}

GLuint* Sampler::create_handle(const std::shared_ptr<GladGLContext> &ctx)
{
    GLuint* handle = new GLuint(0);
    ctx->CreateSamplers(1, handle);
    return handle;
}

void Sampler::handle_deleter(GLuint* handle, GladGLContext& ctx)
{
    ctx.DeleteSamplers(1, handle);
    delete handle;
}