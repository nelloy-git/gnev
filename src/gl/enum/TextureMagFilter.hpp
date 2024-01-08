#pragma once

#include "gl/Ctx.hpp"
#include "util/Log.hpp"

namespace gnev {

enum class TextureMagFilter : GLenum {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR
};

} // namespace gnev

std::ostream& operator<<(std::ostream& os, const gnev::TextureMagFilter& value);

template <>
struct fmtquill::formatter<gnev::TextureMagFilter> : ostream_formatter {};