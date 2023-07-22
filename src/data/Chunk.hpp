#pragma once

#include "data/Vertex.hpp"

#include "gl/GLBufferArray.hpp"
#include "gl/GLBufferVectorT.hpp"
#include "gl/GLVertexArray.hpp"

namespace gnev {

template<VertexT Attr, VertexT Prim>
class Chunk {
public:
    Chunk(const std::shared_ptr<GladGLContext>& ctx,
          const GLuint* primitive_indices, GLuint primitive_indices_count,
          const Prim* primitive_vertices, GLuint primitive_vertices_count);
    virtual ~Chunk();

    template<GLuint primitive_attribindex>
    void bind_primitive(GLint shader_attribindex);

    template<GLuint primitive_attribindex>
    void bind_attrubute(GLint shader_attribindex);

// private:
    GLVertexArray _vao;
    GLBufferArray<GLuint> _prim_ibo;
    GLBufferArray<Prim> _prim_vbo;
    GLBufferVectorT<Attr> _attributes;

};

template<VertexT Attr, VertexT Prim>
Chunk<Attr, Prim>::Chunk(const std::shared_ptr<GladGLContext>& ctx,
                         const GLuint* primitive_indices, GLuint primitive_indices_count,
                         const Prim* primitive_vertices, GLuint primitive_vertices_count) :
    _vao(ctx),
    _prim_ibo(ctx, primitive_indices_count, 0, primitive_indices),
    _prim_vbo(ctx, primitive_vertices_count, 0, primitive_vertices),
    _attributes(ctx, GL_DYNAMIC_DRAW)
{
    _vao.glVertexArrayElementBuffer(_prim_ibo.handle());
    _vao.glVertexArrayVertexBuffer(0, _prim_vbo.handle(), 0, Prim::Size);
    _vao.glVertexArrayVertexBuffer(1, _prim_vbo.handle(), 0, Attr::Size);
}   

template<VertexT Attr, VertexT Prim>
Chunk<Attr, Prim>::~Chunk()
{
}

template<VertexT Attr, VertexT Prim>
template<GLuint primitive_attribindex>
void Chunk<Attr, Prim>::bind_primitive(GLint shader_attribindex)
{
    static constexpr GLuint I = primitive_attribindex;
    static constexpr auto OFFSET = Prim::template get_offset<I>();

    _vao.glVertexArrayAttribBinding(shader_attribindex, 0);
    _vao.glVertexArrayAttribFormat(shader_attribindex, Prim::attrib_t<I>::Elements, Prim::attrib_t<I>::GLType, Prim::attrib_t<I>::Norm, OFFSET);
    _vao.glEnableVertexArrayAttrib(shader_attribindex);
}

template<VertexT Attr, VertexT Prim>
template<GLuint primitive_attribindex>
void Chunk<Attr, Prim>::bind_attrubute(GLint shader_attribindex)
{
    static constexpr GLuint I = primitive_attribindex;
    static constexpr auto OFFSET = Prim::template get_offset<I>();

    _vao.glVertexArrayAttribBinding(shader_attribindex, 0);
    _vao.glVertexArrayAttribFormat(shader_attribindex, Prim::attrib_t<I>::Elements, Prim::attrib_t<I>::GLType, Prim::attrib_t<I>::Norm, OFFSET);
    _vao.glEnableVertexArrayAttrib(shader_attribindex);
}



}