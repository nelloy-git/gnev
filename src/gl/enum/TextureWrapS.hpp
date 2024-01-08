#pragma once

#include "gl/Ctx.hpp"
#include "util/Log.hpp"

namespace gnev {

enum class TextureWrapS : GLenum {
    REPEAT = GL_REPEAT,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
};

} // namespace gnev

std::ostream& operator<<(std::ostream& os, const gnev::TextureWrapS& value);

template <>
struct fmtquill::formatter<gnev::TextureWrapS> : ostream_formatter {};