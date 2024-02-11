#pragma once

#include "gl/Ctx.hpp"

namespace gnev {

enum class TextureFormat : GLenum {
    RED = GL_RED,
    RG = GL_RG,
    RGB = GL_RGB,
    BGR = GL_BGR,
    RGBA = GL_RGBA,
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    STENCIL_INDEX = GL_STENCIL_INDEX
};

std::string_view format_as(TextureFormat value);

} // namespace gnev