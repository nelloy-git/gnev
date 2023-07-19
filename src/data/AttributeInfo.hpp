#pragma once

#include <array>
#include <memory>
#include <variant>

#include "glad/gl.h"

#include "util/Util.hpp"

namespace gnev {

class EXPORT AttributeInfo final {
public:
    AttributeInfo(GLint loc, GLint elements, GLenum type, GLenum normalized);
    ~AttributeInfo();
    
    const GLint loc;
    const GLuint size;
    const GLint elements;
    const GLenum type;
    const GLenum normalized;

    void convert(void* dst, const GLfloat* vec) const;
    void convert(void* dst, const GLint* vec) const;
    void convert(void* dst, const GLuint* vec) const;

private:
    static GLuint _get_attribute_size(GLenum type, GLint elements);

    void _convert_float_to_float(void* dst, const GLfloat* vec) const;
    void _convert_float_to_int(void* dst, const GLfloat* vec) const;
    void _convert_float_to_int_norm(void* dst, const GLfloat* vec) const;
    
    void _convert_int_to_float(void* dst, const GLint* vec) const;
    void _convert_int_to_float_norm(void* dst, const GLint* vec) const;
    void _convert_int_to_int(void* dst, const GLint* vec) const;

    void _convert_uint_to_uint(void* dst, const GLuint* vec) const;

};

}