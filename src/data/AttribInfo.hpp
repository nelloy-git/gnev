#pragma once

#include <stdexcept>

#include "data/AttribType.hpp"

namespace gnev {

struct AttribInfo {
    constexpr AttribInfo(GLuint elements, GLenum type, bool normalized);

    const GLuint elements;
    const GLenum type;
    const bool normalized;
    const bool is_packed;
    const bool is_signed;
    const size_t size;

    static constexpr bool is_packed_type(GLenum type);
    static constexpr bool is_signed_type(GLenum type);
    static constexpr size_t get_type_size(GLuint elements, GLenum type);
};




constexpr AttribInfo::AttribInfo(GLuint elements, GLenum type, bool normalized)
    : elements(elements),
      type(type),
      normalized(normalized),
      is_packed(is_packed_type(type)),
      is_signed(is_signed_type(type)),
      size(get_type_size(elements, type))
{
    if (elements > 4){
        throw std::exception("Vertex attribute can have maximum 4 components");
    }

    if (elements == 0){
        throw std::exception("Vertex attribute must have at least 1 component");
    }

    if (type == GL_UNSIGNED_INT_10_10_10_2 && elements != 4){
        throw std::exception("UInt_10_10_10_2 can be 4 components only");
    }
};

constexpr bool AttribInfo::is_packed_type(GLenum type)
{
    switch (type)
    {
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
        throw std::exception("Unknown attribute type");
    }
}

constexpr bool AttribInfo::is_signed_type(GLenum type)
{
    switch (type)
    {
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
        throw std::exception("Unknown attribute type");
    }
}

constexpr size_t AttribInfo::get_type_size(GLuint elements, GLenum type)
{
    switch (type)
    {
    case GL_FLOAT: return elements * sizeof(GLfloat);
    case GL_BYTE: return elements * sizeof(GLbyte);
    case GL_UNSIGNED_BYTE: return elements * sizeof(GLubyte);
    case GL_SHORT: return elements * sizeof(GLshort);
    case GL_UNSIGNED_SHORT: return elements * sizeof(GLushort);
    case GL_INT: return elements * sizeof(GLint);
    case GL_UNSIGNED_INT: return elements * sizeof(GLuint);

    case GL_UNSIGNED_INT_10_10_10_2: return sizeof(GLuint);
    
    default:
        throw std::exception("Unknown attribute type");
    }
}

}