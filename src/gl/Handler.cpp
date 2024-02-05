#include "gl/Handler.hpp"

#include <memory>
#include <stdexcept>

#include "gl/logger/HandlerLogger.hpp"

namespace gnev::gl {

Handler::Handler(GLuint* handle, void (*deleter)(GLuint*))
    : _handle(std::unique_ptr<GLuint, void (*)(GLuint*)>(handle, deleter)) {
    if (!_handle) {
        throw std::runtime_error("");
    }
}

std::unique_ptr<HandlerLogger> Handler::Log(const SrcLoc& src_loc) const {
    return std::make_unique<HandlerLogger>(src_loc, *_handle);
}

} // namespace gnev::gl