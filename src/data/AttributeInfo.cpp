#include "data/AttributeInfo.hpp"

#include <stdexcept>
#include <limits>

using namespace gnev;

AttributeInfo::AttributeInfo(GLint loc, GLint elements, GLenum type, GLenum normalized) :
    loc(loc),
    size(_get_attribute_size(type, elements)),
    elements(elements),
    type(type),
    normalized(normalized)
{
}

AttributeInfo::~AttributeInfo()
{
}

void AttributeInfo::convert(void* dst, const GLfloat* vec) const
{
    if (normalized){
        switch (type)
        {
        case GL_INT:
            _convert_float_to_int_norm(dst, vec);
            break;
        default:
            throw std::logic_error("Unsupported convertion");
        }
    } else {
        switch (type)
        {
        case GL_INT:
            _convert_float_to_int(dst, vec);
            break;
        case GL_FLOAT:
            _convert_float_to_float(dst, vec);
            break;
        default:
            throw std::logic_error("Unsupported convertion");
        }
    }
}

void AttributeInfo::convert(void* dst, const GLint* vec) const
{
    if (normalized){
        switch (type)
        {
        case GL_INT:
            _convert_int_to_float_norm(dst, vec);
            break;
        default:
            throw std::logic_error("Unsupported convertion");
        }
    } else {
        switch (type)
        {
        case GL_INT:
            _convert_int_to_int(dst, vec);
            break;
        case GL_FLOAT:
            _convert_int_to_float(dst, vec);
            break;
        default:
            throw std::logic_error("Unsupported convertion");
        }
    }
}

void AttributeInfo::convert(void* dst, const GLuint* vec) const
{
    if (normalized){
        switch (type)
        {
        default:
            throw std::logic_error("Unsupported convertion");
        }
    } else {
        switch (type)
        {
        case GL_UNSIGNED_INT:
            _convert_uint_to_uint(dst, vec);
            break;
        default:
            throw std::logic_error("Unsupported convertion");
        }
    }
}

GLuint AttributeInfo::_get_attribute_size(GLenum type, GLint elements) {
    switch (type){
        case GL_BYTE: return elements * sizeof(GLbyte); break;
        case GL_SHORT: return elements * sizeof(GLshort); break;
        case GL_INT: return elements * sizeof(GLint); break;
        case GL_FIXED: return elements * sizeof(GLfixed); break;
        case GL_FLOAT: return elements * sizeof(GLfloat); break;
        case GL_HALF_FLOAT: return elements * sizeof(GLhalf); break;
        case GL_DOUBLE: return elements * sizeof(GLdouble); break;
        case GL_UNSIGNED_BYTE: return elements * sizeof(GLubyte); break;
        case GL_UNSIGNED_SHORT: return elements * sizeof(GLushort); break;
        case GL_UNSIGNED_INT: return elements * sizeof(GLuint); break;
        // case GL_INT_2_10_10_10_REV: return sizeof(GLuint); break;
        // case GL_UNSIGNED_INT_2_10_10_10_REV: return sizeof(GLuint); break;
        // case GL_UNSIGNED_INT_10F_11F_11F_REV : return sizeof(GLbyte); break;
    default:
        throw std::logic_error("Unknown attribute format type");
    }
}

void AttributeInfo::_convert_float_to_float(void* dst, const GLfloat* vec) const
{
    GLfloat* data = static_cast<GLfloat*>(dst);
    for (int i = 0; i < elements; ++i){
        data[i] = vec[i];
    }
}

void AttributeInfo::_convert_float_to_int(void* dst, const GLfloat* vec) const
{
    GLint* data = static_cast<GLint*>(dst);
    for (int i = 0; i < elements; ++i){
        data[i] = vec[i];
    }
}

void AttributeInfo::_convert_float_to_int_norm(void* dst, const GLfloat* vec) const
{
    GLint* data = static_cast<GLint*>(dst);
    for (int i = 0; i < elements; ++i){
        data[i] = vec[i] * std::numeric_limits<GLint>::max();
    }
}

void AttributeInfo::_convert_int_to_float(void* dst, const GLint* vec) const
{
    GLfloat* data = static_cast<GLfloat*>(dst);
    for (int i = 0; i < elements; ++i){
        data[i] = vec[i];
    }
}

void AttributeInfo::_convert_int_to_float_norm(void* dst, const GLint* vec) const
{
    GLfloat* data = static_cast<GLfloat*>(dst);
    for (int i = 0; i < elements; ++i){
        data[i] = vec[i] / std::numeric_limits<GLint>::max();
    }
}

void AttributeInfo::_convert_int_to_int(void* dst, const GLint* vec) const
{
    GLint* data = static_cast<GLint*>(dst);
    for (int i = 0; i < elements; ++i){
        data[i] = vec[i];
    }
}

void AttributeInfo::_convert_uint_to_uint(void* dst, const GLuint* vec) const
{
    GLuint* data = static_cast<GLuint*>(dst);
    for (int i = 0; i < elements; ++i){
        data[i] = vec[i];
    }
}