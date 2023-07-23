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

    static constexpr bool is_packed_type(GLenum type);
    static constexpr bool is_signed_type(GLenum type);
};




constexpr AttribInfo::AttribInfo(GLuint elements, GLenum type, bool normalized)
    : elements(elements),
      type(type),
      normalized(normalized),
      is_packed(is_packed_type(type)),
      is_signed(is_signed_type(type))
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

constexpr bool AttribInfo::is_packed_type(GLenum type){
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

constexpr bool AttribInfo::is_signed_type(GLenum type){
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

}