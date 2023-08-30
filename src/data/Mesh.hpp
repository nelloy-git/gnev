#pragma once

#include "data/Vertex.hpp"
#include "gl/BufferVector.hpp"
#include "gl/VertexArray.hpp"

namespace gnev
{

template<IsIndex I, IsVertex V>
class Mesh {
public:
    static constexpr GLenum IndexEnum = std::is_same_v<I, GLubyte> ? GL_UNSIGNED_BYTE
                                       : std::is_same_v<I, GLushort> ? GL_UNSIGNED_SHORT
                                       : std::is_same_v<I, GLuint> ? GL_UNSIGNED_INT : 0;

    using index_type = I;
    using vertex_type = V;

    Mesh(const gl::GladCtx& ctx);
    virtual ~Mesh();

    void bind_attribute(GLint shader_loc, GLint attribute_loc);
    
    inline gl::VertexArray vao() const {return _vao;}
    inline gl::BufferVector<I> indices() const {return _indices;};
    inline gl::BufferVector<V> vertices() const {return _vertices;};

private:
    gl::VertexArray _vao;
    gl::BufferVector<I> _indices;
    gl::BufferVector<V> _vertices;
};

template<IsIndex I, IsVertex V>
Mesh<I, V>::Mesh(const gl::GladCtx& ctx)
    : _vao(ctx),
      _indices(ctx, 0, nullptr),
      _vertices(ctx, 0, nullptr)
{
    _vao.glVertexArrayElementBuffer(_indices.handle());
    _vao.glVertexArrayVertexBuffer(0, _vertices.handle(), 0, sizeof(vertex_type));
}

template<IsIndex I, IsVertex V>
Mesh<I, V>::~Mesh()
{
}

template<IsIndex I, IsVertex V>
void Mesh<I, V>::bind_attribute(GLint shader_loc, GLint attribute_loc)
{
    static constexpr VertexInfo vertex_info = vertex_type::info;
    if (shader_loc < 0 || attribute_loc < 0 || attribute_loc >= vertex_info.count){
        throw std::out_of_range("");
    }
    const auto attr_info = vertex_info.attributes[attribute_loc];
    _vao.glVertexArrayAttribBinding(shader_loc, 0);
    _vao.glVertexArrayAttribFormat(shader_loc, attr_info.elements, attr_info.type, attr_info.normalized, vertex_info.offsets[attribute_loc]);
    _vao.glEnableVertexArrayAttrib(shader_loc);

    std::cout << "bind_attribute: " << std::endl
              << "\tshader_loc" << shader_loc << std::endl
              << "\tattr_info.elements" << attr_info.elements << std::endl
              << "\tattr_info.type" << attr_info.type << std::endl
              << "\tattr_info.normalized" << attr_info.normalized << std::endl
              << "\tvertex_info.offsets[attribute_loc]" << vertex_info.offsets[attribute_loc] << std::endl;

}

}