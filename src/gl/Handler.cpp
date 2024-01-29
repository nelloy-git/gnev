#include "gl/Handler.hpp"

#include <memory>
#include <stdexcept>

#include "gl/fmt/HandlerLog.hpp"

namespace gnev::gl {

Handler::Handler(GLuint* handle, void (*deleter)(GLuint*))
    : _handle(std::unique_ptr<GLuint, void (*)(GLuint*)>(handle, deleter)) {
    if (!_handle) {
        throw std::runtime_error("");
    }
}

std::unique_ptr<HandlerLog> Handler::Log(const CtString<>& class_name,
                                         const CtString<>& method_name) const {
    return std::make_unique<HandlerLog>(class_name, method_name, *_handle);
}
} // namespace gnev::gl