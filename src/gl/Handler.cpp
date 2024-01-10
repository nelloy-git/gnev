#include "gl/Handler.hpp"

#include <memory>
#include <stdexcept>

#include "gl/fmt/HandlerTraceL2.hpp"

namespace gnev::gl {

Handler::Handler(GLuint* handle, void (*deleter)(GLuint*))
    : _handle(std::unique_ptr<GLuint, void (*)(GLuint*)>(handle, deleter)) {
    if (!_handle) {
        throw std::runtime_error("");
    }
}

std::unique_ptr<HandlerTraceL2> Handler::L2(const CtString<128>& class_name,
                                            const CtString<128>& method_name) const {
    return std::make_unique<HandlerTraceL2>(handle(), class_name, method_name);
}
} // namespace gnev::gl