#include "gl/GLHandler.hpp"

using namespace gnev;

GLHandler::GLHandler(const std::shared_ptr<GladGLContext>& ctx, GLuint* handle, const Deleter& deleter) :
    _ctx(ctx),
    _handle(handle, [ctx, deleter](GLuint* handle){
        deleter(handle, *ctx);
    }){
}

GLHandler::~GLHandler(){
}

const std::shared_ptr<GladGLContext>& GLHandler::ctx() const {
    return _ctx;
}

GLuint GLHandler::handle() const {
    return *_handle;
}