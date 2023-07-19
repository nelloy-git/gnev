#include "AttributeSettings.hpp"

#include <stdexcept>

using namespace gnev;

AttributeSettings::AttributeSettings(GLuint max_attributes){
    _attributes.reserve(max_attributes);
}

AttributeSettings::~AttributeSettings(){
}

void AttributeSettings::enable(GLuint attrib_index){
    _attributes[attrib_index].enabled = true;
}

void AttributeSettings::disable(GLuint attrib_index){
    _attributes[attrib_index].enabled = false;
}

void AttributeSettings::set(GLuint attrib_index,
                            GLint format_size, GLenum format_type, GLboolean format_normalized, GLuint format_relative_offset,
                            GLuint divisor){
    _attributes[attrib_index].format_size = format_size;
    _attributes[attrib_index].format_type = format_type;
    _attributes[attrib_index].format_normalized = format_normalized;
    _attributes[attrib_index].format_relative_offset = format_relative_offset;
    _attributes[attrib_index].divisor = divisor;
    _attributes[attrib_index].attribute_size = _get_format_type_size(format_type, format_size);
}

void AttributeSettings::apply(GLuint attrib_index, GLVertexArray& vao, GLBuffer& buffer) const {
    auto& attrib = _attributes.at(attrib_index);
    if (!attrib.enabled){
        vao.glDisableVertexArrayAttrib(attrib_index);
        return;
    }

    vao.glEnableVertexArrayAttrib(attrib_index);
    vao.glVertexArrayAttribBinding(attrib_index, attrib.buffer_binding);
    vao.glVertexArrayVertexBuffer(attrib.buffer_binding, buffer.handle(), attrib.buffer_offset, attrib.buffer_stride);
    vao.glVertexArrayAttribFormat(attrib_index, attrib.format_size, attrib.format_type, attrib.format_normalized, attrib.format_relative_offset);
    vao.glVertexArrayBindingDivisor(attrib.buffer_binding, attrib.divisor);
}

GLuint AttributeSettings::get_vertex_size() const {
    GLuint s = 0;
    for (auto& attribute : _attributes){
        s += attribute.attribute_size;
    }
    return s;
}

GLuint AttributeSettings::_get_format_type_size(GLenum format_type, GLint format_size){
    switch (format_type){
        case GL_BYTE: return format_size * sizeof(GLbyte); break;
        case GL_SHORT: return format_size * sizeof(GLshort); break;
        case GL_INT: return format_size * sizeof(GLint); break;
        case GL_FIXED: return format_size * sizeof(GLfixed); break;
        case GL_FLOAT: return format_size * sizeof(GLfloat); break;
        case GL_HALF_FLOAT: return format_size * sizeof(GLhalf); break;
        case GL_DOUBLE: return format_size * sizeof(GLdouble); break;
        case GL_UNSIGNED_BYTE: return format_size * sizeof(GLubyte); break;
        case GL_UNSIGNED_SHORT: return format_size * sizeof(GLushort); break;
        case GL_UNSIGNED_INT: return format_size * sizeof(GLuint); break;
        case GL_INT_2_10_10_10_REV: return sizeof(GLuint); break;
        case GL_UNSIGNED_INT_2_10_10_10_REV: return sizeof(GLuint); break;
        case GL_UNSIGNED_INT_10F_11F_11F_REV : return sizeof(GLbyte); break;
    default:
        throw std::logic_error("Unknown attribute format type");
    }
}