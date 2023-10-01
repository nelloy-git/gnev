#include "gl/Handler.hpp"

using namespace gnev::gl;

Handler::Handler(const Ctx& ctx, GLuint handle, const FreeHandle& deleter)
    : _ctx(ctx)
    , _handle(std::make_unique<GLuint>(handle))
    , _deleter(deleter) {}

Handler::~Handler() {
    if (_handle) {
        _deleter(_ctx, *_handle);
    }
}