#pragma once

#include "data/Vertex.hpp"
#include "gl/BufferVector.hpp"
#include "gl/VertexArray.hpp"

namespace gnev
{

template<typename I, IsVertex V>
class Mesh
{
    static_assert(std::is_same_v<I, GLubyte> || std::is_same_v<I, GLushort> || std::is_same_v<I, GLuint>, "Index type is not valid");

public:
    static constexpr GLenum IndexEnum = std::is_same_v<I, GLubyte> ? GL_UNSIGNED_BYTE
                                       : std::is_same_v<I, GLushort> ? GL_UNSIGNED_SHORT
                                       : std::is_same_v<I, GLuint> ? GL_UNSIGNED_INT : 0;

    using index_type = I;
    using vertex_type = V;

    Mesh(const std::shared_ptr<GladGLContext> &ctx);
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

template<typename I, IsVertex V>
Mesh<I, V>::Mesh(const std::shared_ptr<GladGLContext> &ctx)
    : _vao(ctx),
      _indices(ctx, 0, nullptr),
      _vertices(ctx, 0, nullptr)
{
}

template<typename I, IsVertex V>
Mesh<I, V>::~Mesh()
{
}

template<typename I, IsVertex V>
void Mesh<I, V>::bind_attribute(GLint shader_loc, GLint attribute_loc)
{
    static constexpr VertexInfo vertex_info = vertex_type::info;
    if (shader_loc < 0 || attribute_loc < 0 || attribute_loc >= vertex_info.count){
        throw std::out_of_range("");
    }
    const auto attr_info = vertex_info.attributes[attribute_loc];
    _vao.glVertexArrayAttribBinding(shader_loc, 0);
    _vao.glVertexArrayAttribFormat(shader_loc, attr_info.size, attr_info.type, attr_info.normalized, vertex_info.offsets[attribute_loc]);
    _vao.glEnableVertexArrayAttrib(shader_loc);
}

}