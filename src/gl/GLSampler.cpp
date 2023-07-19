#include "gl/GLSampler.hpp"

using namespace gnev;

GLSampler::GLSampler(const std::shared_ptr<GladGLContext> &ctx) :
    GLHandler(ctx, create_handle(ctx), &handle_deleter)
{
}

GLSampler::~GLSampler()
{
}

void GLSampler::glBindSampler(GLuint unit)
{
    ctx()->BindSampler(unit, handle());
}

void GLSampler::glSamplerParameteri(GLuint pname, GLint param)
{
    ctx()->SamplerParameteri(handle(), pname, param);
}

void GLSampler::glSamplerParameterf(GLuint pname, GLfloat param)
{
    ctx()->SamplerParameterf(handle(), pname, param);
}

void GLSampler::glSamplerParameterfv(GLuint pname, const GLfloat* param)
{
    ctx()->SamplerParameterfv(handle(), pname, param);
}

GLuint* GLSampler::create_handle(const std::shared_ptr<GladGLContext> &ctx)
{
    GLuint* handle = new GLuint(0);
    ctx->CreateSamplers(1, handle);
    return handle;
}

void GLSampler::handle_deleter(GLuint* handle, GladGLContext& ctx)
{
    ctx.DeleteSamplers(1, handle);
    delete handle;
}