#pragma once

#include <optional>
#include <vector>

#include "gl/GLBuffer.hpp"
#include "gl/GLProgram.hpp"
#include "gl/GLVertexArray.hpp"

namespace gnev {

class AttributeSettings {
public:
    AttributeSettings(GLuint max_attributes);
    ~AttributeSettings();

    void enable(GLuint attrib_index);
    void disable(GLuint attrib_index);
    void set(GLuint attrib_index,
             GLint format_size, GLenum format_type, GLboolean format_normalized, GLuint format_relative_offset,
             GLuint divisor);
    void apply(GLuint attrib_index, GLVertexArray& vao, GLBuffer& buffer) const;

    GLuint get_vertex_size() const;

private:
    struct Attribute {
        bool enabled = false;
        
        GLuint buffer_binding;
        GLintptr buffer_offset;
        GLsizei buffer_stride;

        GLint format_size;
        GLenum format_type;
        GLboolean format_normalized;
        GLuint format_relative_offset;

        GLuint divisor;

        GLuint attribute_size;
    };

    int _used_locations = 0;

    std::vector<Attribute> _attributes;

    static GLuint _get_format_type_size(GLenum format_type, GLint format_size);
};

}