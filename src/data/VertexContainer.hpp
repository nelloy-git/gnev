#pragma once

#include "gl/GLBufferVector.hpp"
#include "gl/GLVertexArray.hpp"
#include "data/AttribInfo.hpp"

namespace gnev {

class EXPORT VertexContainer {
public:
    VertexContainer(const VertexContainer&) = default;
    VertexContainer(const std::shared_ptr<GladGLContext> &ctx, GLenum index_type, const std::initializer_list<AttribInfo>& vertex_info);
    virtual ~VertexContainer();

    const GLenum index_type;
    const GLenum index_size;
    const std::vector<AttribInfo> vertex_info;
    const GLuint vertex_size;

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
    static GLuint _get_vertex_size(const std::initializer_list<AttribInfo>& vertex_info);
};

}