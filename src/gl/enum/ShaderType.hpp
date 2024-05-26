#pragma once

#include "Ctx.hpp"
#include "magic_enum/magic_enum.hpp"

namespace gnev::gl {

enum class ShaderType : GLenum {
    COMPUTE_SHADER = GL_COMPUTE_SHADER,
    VERTEX_SHADER = GL_VERTEX_SHADER,
    TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
    TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
    GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
};

std::string_view format_as(ShaderType value);

} // namespace gnev::gl

template<>
struct magic_enum::customize::enum_range<gnev::gl::ShaderType> {
    static constexpr int min = 35000;
    static constexpr int max = 38000;
};
