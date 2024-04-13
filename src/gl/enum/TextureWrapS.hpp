#pragma once

#include "gl/Ctx.hpp"

namespace gnev::gl {

enum class TextureWrapS : GLenum {
    REPEAT = GL_REPEAT,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
};

std::string_view format_as(TextureWrapS value);

} // namespace gnev