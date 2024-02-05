#pragma once

#include "gl/Ctx.hpp"
#include "util/Logger.hpp"

namespace gnev {

enum class TextureMinFilter : GLenum {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

std::ostream& operator<<(std::ostream& os, const gnev::TextureMinFilter& value);

} // namespace gnev

template <>
struct fmtquill::formatter<gnev::TextureMinFilter> : ostream_formatter {};