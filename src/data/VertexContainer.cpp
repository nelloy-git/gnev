#include "data/VertexContainer.hpp"

using namespace gnev;

VertexContainer::VertexContainer(const std::shared_ptr<GladGLContext> &ctx,
                                 GLenum index_type, const VertexInfo& vertex_info)
    : index_type(index_type),
      index_size(_get_index_size(index_type)),
      vertex_info(vertex_info),
      _vao(ctx),
      _indices(ctx, nullptr, 0, GL_DYNAMIC_COPY),
      _vertices(ctx, nullptr, 0, GL_DYNAMIC_COPY)
{
    _vao.glVertexArrayElementBuffer(_indices.handle());
    _vao.glVertexArrayVertexBuffer(0, _vertices.handle(), 0, vertex_info.size);
}

VertexContainer::~VertexContainer()
{
}

void VertexContainer::bind_attribute(GLint shader_loc, GLint attribute_loc)
{
    if (attribute_loc < 0 || attribute_loc >= vertex_info.count){
        throw std::out_of_range("");
    }

    const auto& info = vertex_info.attributes[attribute_loc];
    _vao.glVertexArrayAttribBinding(shader_loc, 0);
    _vao.glVertexArrayAttribFormat(shader_loc, info.elements, info.type, info.normalized, vertex_info.offsets[attribute_loc]);
    _vao.glEnableVertexArrayAttrib(shader_loc);
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

// GLuint VertexContainer::_get_vertex_size(const std::initializer_list<VertexAttributeInfo>& vertex_info)
// {
//     GLuint size = 0;
//     for (auto& attrib : vertex_info){
//         size += attrib.size;
//     }
//     return size;
// }