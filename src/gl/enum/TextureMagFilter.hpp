#pragma once

#include "gl/Ctx.hpp"
#include "util/Logger.hpp"

namespace gnev {

enum class TextureMagFilter : GLenum {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR
};

std::ostream& operator<<(std::ostream& os, const TextureMagFilter& value);

} // namespace gnev

template <>
struct fmtquill::formatter<gnev::TextureMagFilter> : ostream_formatter {};