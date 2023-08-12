#pragma once

#include "gl/GLBufferVector.hpp"
#include "gl/GLVertexArray.hpp"
#include "data/VertexInfo.hpp"

namespace gnev {

class EXPORT VertexContainer {
public:
    VertexContainer(const VertexContainer&) = default;
    VertexContainer(const std::shared_ptr<GladGLContext> &ctx, GLenum index_type, const VertexInfo& vertex_info);
    virtual ~VertexContainer();

    const GLenum index_type;
    const unsigned int index_size;
    const VertexInfo& vertex_info;

    void bind_attribute(GLint shader_loc, GLint attribute_loc);

    GLVertexArray& vao();
    const GLVertexArray& vao() const;

    GLBufferVector& indices();
    const GLBufferVector& indices() const;

    GLBufferVector& vertices();
    const GLBufferVector& vertices() const;

private:
    GLVertexArray _vao;
    GLBufferVector _indices;
    GLBufferVector _vertices;

    static GLuint _get_index_size(GLenum index_type);
    // static GLuint _get_vertex_size(const std::initializer_list<VertexAttributeInfo>& vertex_info);
};

}