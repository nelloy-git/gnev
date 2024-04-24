#pragma once

#include "Ctx.hpp"

namespace gnev::gl {

enum class TextureMagFilter : GLenum {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR
};

std::string_view format_as(TextureMagFilter value);

} // namespace gnev::gl
