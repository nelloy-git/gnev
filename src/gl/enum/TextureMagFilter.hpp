#pragma once

#include "gl/Ctx.hpp"

namespace gnev {

enum class TextureMagFilter : GLenum {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR
};

std::string_view format_as(TextureMagFilter value);

} // namespace gnev
