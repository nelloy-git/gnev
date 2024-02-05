#pragma once

#include "gl/Ctx.hpp"
#include "util/Logger.hpp"

namespace gnev {

enum class TextureWrapT : GLenum {
    REPEAT = GL_REPEAT,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
};

std::ostream& operator<<(std::ostream& os, const gnev::TextureWrapT& value);

} // namespace gnev

template <>
struct fmtquill::formatter<gnev::TextureWrapT> : ostream_formatter {};