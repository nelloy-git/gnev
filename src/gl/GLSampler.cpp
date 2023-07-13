#include "gl/GLSampler.hpp"

using namespace gnev;

GLSampler::GLSampler(const std::shared_ptr<GladGLContext> &ctx) :
    handle(create_sampler(ctx)),
    ctx(ctx){

}

GLSampler::~GLSampler(){
    ctx->DeleteSamplers(1, &handle);
}

void GLSampler::glBindSampler(GLuint unit){
    ctx->BindSampler(unit, handle);
}

void GLSampler::glSamplerParameteri(GLuint pname, GLint param){
    ctx->SamplerParameteri(handle, pname, param);
}

void GLSampler::glSamplerParameterf(GLuint pname, GLfloat param){
    ctx->SamplerParameterf(handle, pname, param);
}

void GLSampler::glSamplerParameterfv(GLuint pname, const GLfloat* param){
    ctx->SamplerParameterfv(handle, pname, param);
}

GLuint GLSampler::create_sampler(const std::shared_ptr<GladGLContext> &ctx){
    GLuint handle;
    ctx->CreateSamplers(1, &handle);
    return handle;
}