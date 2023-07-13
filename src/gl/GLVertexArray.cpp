#include "gl/GLVertexArray.hpp"

using namespace gnev;

GLVertexArray::GLVertexArray(const std::shared_ptr<GladGLContext> &ctx) :
    handle(create_vertex_array(ctx)),
    ctx(ctx){
}

GLVertexArray::~GLVertexArray(){
    ctx->DeleteVertexArrays(1, &handle);
}

void GLVertexArray::glBindVertexArray() const {
    ctx->BindVertexArray(handle);
}

GLuint GLVertexArray::create_vertex_array(const std::shared_ptr<GladGLContext> &ctx){
    GLuint handle;
    if (ctx->VERSION_4_5){
        ctx->CreateVertexArrays(1, &handle);
    } else {
        ctx->GenVertexArrays(1, &handle);
    }
    return handle;
}
