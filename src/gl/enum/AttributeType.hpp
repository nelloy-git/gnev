#pragma once

#include "Ctx.hpp"

namespace gnev::gl {

enum class AttributeType : GLenum {
    BYTE = GL_BYTE,
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    SHORT = GL_SHORT,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    INT = GL_INT,
    UNSIGNED_INT = GL_UNSIGNED_INT,
    FIXED = GL_FIXED,
    FLOAT = GL_FLOAT,
    HALF_FLOAT = GL_HALF_FLOAT,
    DOUBLE = GL_DOUBLE,
    INT_2_10_10_10_REV = GL_INT_2_10_10_10_REV,
    UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
    UNSIGNED_INT_10F_11F_11F_REV = GL_UNSIGNED_INT_10F_11F_11F_REV
};

std::string_view format_as(AttributeType value);

} // namespace gnev::gl