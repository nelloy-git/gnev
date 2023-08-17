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
    static constexpr GLenum IndexEnum = std::is_same_v<I, GLubyte> ? GL_UNSIGNED_BYTE
                                       : std::is_same_v<I, GLushort> ? GL_UNSIGNED_SHORT
                                       : std::is_same_v<I, GLuint> ? GL_UNSIGNED_INT : 0;

public:
    using index_type = I;
    using vertex_type = V;

    Mesh(const std::shared_ptr<GladGLContext> &ctx);
    virtual ~Mesh();
    
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
      _indices(ctx, nullptr, 0),
      _vertices(ctx, nullptr, 0)
{
}

template<typename I, IsVertex V>
Mesh<I, V>::~Mesh()
{
}

}