#pragma once

#include "gl/Ctx.hpp"
#include "util/Logger.hpp"

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

std::ostream& operator<<(std::ostream& os, const TextureFormat& value);

} // namespace gnev

template <>
struct fmtquill::formatter<gnev::TextureFormat> : ostream_formatter {};