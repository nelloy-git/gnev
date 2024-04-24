#pragma once

#include "Ctx.hpp"

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
