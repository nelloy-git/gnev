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

HandlerLogger Handler::getLogger(const SrcLoc& src_loc) const {
    return HandlerLogger{std::move(Ctx::Get().getLogger(src_loc)), *_handle};
}

} // namespace gnev::gl