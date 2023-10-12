#include "gl/Handler.hpp"

#include <memory>
#include <stdexcept>

using namespace gnev::gl;

Handler::Handler(GLuint* handle, void (*deleter)(GLuint*))
    : _handle(std::unique_ptr<GLuint, void (*)(GLuint*)>(handle, deleter)) {
    if (!_handle) {
        throw std::runtime_error("");
    }
}

Handler::~Handler() {}