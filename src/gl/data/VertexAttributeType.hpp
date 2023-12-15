#pragma once

#include <array>

#include "glad/gl.h"

namespace gnev::gl {

template <GLenum T, GLuint S>
struct VertexAttributeType {};

template <GLuint S>
struct VertexAttributeType<GL_FLOAT, S> {
    using type = std::array<GLfloat, S>;
    using element_type = GLfloat;
};

template <GLuint S>
struct VertexAttributeType<GL_INT, S> {
    using type = std::array<GLint, S>;
    using element_type = GLint;
};

template <GLuint S>
struct VertexAttributeType<GL_BYTE, S> {
    using type = std::array<GLbyte, S>;
    using element_type = GLbyte;
};

template <GLuint S>
struct VertexAttributeType<GL_SHORT, S> {
    using type = std::array<GLshort, S>;
    using element_type = GLshort;
};

template <GLuint S>
struct VertexAttributeType<GL_UNSIGNED_INT, S> {
    using type = std::array<GLuint, S>;
    using element_type = GLuint;
};

template <GLuint S>
struct VertexAttributeType<GL_UNSIGNED_BYTE, S> {
    using type = std::array<GLubyte, S>;
    using element_type = GLubyte;
};

template <GLuint S>
struct VertexAttributeType<GL_UNSIGNED_SHORT, S> {
    using type = std::array<GLushort, S>;
    using element_type = GLushort;
};

template <>
struct VertexAttributeType<GL_UNSIGNED_INT_10_10_10_2, 4> {
    using type = std::array<GLuint, 1>;
};

} // namespace gnev::gl::data