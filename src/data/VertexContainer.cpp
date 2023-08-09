#include "data/VertexContainer.hpp"

using namespace gnev;

VertexContainer::VertexContainer(const std::shared_ptr<GladGLContext> &ctx,
                                 GLenum index_type, const std::initializer_list<AttribInfo>& vertex_info)
    : index_type(index_type),
      index_size(_get_index_size(index_type)),
      vertex_info(vertex_info),
      vertex_size(_get_vertex_size(vertex_info)),
      _vao(ctx),
      _indices(ctx, nullptr, 0, GL_DYNAMIC_COPY),
      _vertices(ctx, nullptr, 0, GL_DYNAMIC_COPY)
{
    _vao.glVertexArrayElementBuffer(_indices.handle());
    _vao.glVertexArrayVertexBuffer(0, _vertices.handle(), 0, vertex_size);
}

VertexContainer::~VertexContainer()
{
}

GLVertexArray& VertexContainer::vao()
{
    return _vao;
}

const GLVertexArray& VertexContainer::vao() const
{
    return _vao;
}

GLBufferVector& VertexContainer::indices()
{
    return _indices;
}

const GLBufferVector& VertexContainer::indices() const
{
    return _indices;
}

GLBufferVector& VertexContainer::vertices()
{
    return _vertices;
}

const GLBufferVector& VertexContainer::vertices() const
{
    return _vertices;
}

GLuint VertexContainer::_get_index_size(GLenum index_type)
{
    switch (index_type)
    {
    case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
    case GL_UNSIGNED_SHORT: return sizeof(GLushort);
    case GL_UNSIGNED_INT: return sizeof(GLuint);
    default:
        throw std::logic_error("");
    }
}

GLuint VertexContainer::_get_vertex_size(const std::initializer_list<AttribInfo>& vertex_info)
{
    GLuint size = 0;
    for (auto& attrib : vertex_info){
        size += attrib.size;
    }
    return size;
}