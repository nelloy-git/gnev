#pragma once

#include "gl/Ctx.hpp"
#include "util/Log.hpp"

namespace gnev {

enum class TextureMinFilter : GLenum {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

} // namespace gnev

std::ostream& operator<<(std::ostream& os, const gnev::TextureMinFilter& value);

template <>
struct fmtquill::formatter<gnev::TextureMinFilter> : ostream_formatter {};