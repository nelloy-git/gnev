#pragma once

#include <stdexcept>

#include "gl/data/VertexAttributeType.hpp"

namespace gnev::gl::data {

struct VertexAttributeInfo {
    constexpr VertexAttributeInfo(GLuint elements, GLenum type, bool normalized)
        : elements(elements)
        , type(type)
        , normalized(normalized)
        , is_packed(is_packed_type(type))
        , is_signed(is_signed_type(type))
        , size(get_type_size(elements, type)) {
        validateParams(elements, type, normalized);
    };

    GLuint elements;
    GLenum type;
    bool normalized;
    bool is_packed;
    bool is_signed;
    size_t size;

private:
    static constexpr void validateParams(GLuint elements, GLenum type, bool normalized) {
        if (elements > 4) {
            throw std::logic_error("Vertex attribute can have maximum 4 components");
        }

        if (elements == 0) {
            throw std::logic_error("Vertex attribute must have at least 1 component");
        }

        if (type == GL_UNSIGNED_INT_10_10_10_2 && elements != 4) {
            throw std::logic_error("UInt_10_10_10_2 can be 4 components only");
        }
    };

    static constexpr bool is_packed_type(GLenum type) {
        switch (type) {
        case GL_FLOAT:
        case GL_INT:
        case GL_BYTE:
        case GL_SHORT:
        case GL_UNSIGNED_INT:
        case GL_UNSIGNED_BYTE:
        case GL_UNSIGNED_SHORT:
            return false;

        case GL_UNSIGNED_INT_10_10_10_2:
            return true;

        default:
            throw std::logic_error("Unknown attribute type");
        }
    };

    static constexpr bool is_signed_type(GLenum type);
    static constexpr size_t get_type_size(GLuint elements, GLenum type);
};

constexpr bool VertexAttributeInfo::is_signed_type(GLenum type) {
    switch (type) {
    case GL_FLOAT:
    case GL_INT:
    case GL_BYTE:
    case GL_SHORT:
        return true;

    case GL_UNSIGNED_INT:
    case GL_UNSIGNED_BYTE:
    case GL_UNSIGNED_SHORT:
    case GL_UNSIGNED_INT_10_10_10_2:
        return false;

    default:
        throw std::logic_error("Unknown attribute type");
    }
}

constexpr size_t VertexAttributeInfo::get_type_size(GLuint elements, GLenum type) {
    switch (type) {
    case GL_FLOAT:
        return elements * sizeof(GLfloat);
    case GL_BYTE:
        return elements * sizeof(GLbyte);
    case GL_UNSIGNED_BYTE:
        return elements * sizeof(GLubyte);
    case GL_SHORT:
        return elements * sizeof(GLshort);
    case GL_UNSIGNED_SHORT:
        return elements * sizeof(GLushort);
    case GL_INT:
        return elements * sizeof(GLint);
    case GL_UNSIGNED_INT:
        return elements * sizeof(GLuint);

    case GL_UNSIGNED_INT_10_10_10_2:
        return sizeof(GLuint);

    default:
        throw std::logic_error("Unknown attribute type");
    }
}

} // namespace gnev::gl::data