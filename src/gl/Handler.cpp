#include "gl/Handler.hpp"

using namespace gnev::gl;

Handler::Handler(const std::shared_ptr<GladGLContext>& ctx, GLuint* handle, const Deleter& deleter)
    : _ctx(ctx)
    , _handle(handle, [ctx, deleter](GLuint* handle) { deleter(handle, *ctx); }) {}

Handler::~Handler() {}

const std::shared_ptr<GladGLContext>& Handler::ctx() const { return _ctx; }

GLuint Handler::handle() const { return *_handle; }