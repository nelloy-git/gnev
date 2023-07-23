#pragma once

#include "data/Vertex.hpp"

#include "gl/GLBufferArrayT.hpp"
#include "gl/GLBufferVectorT.hpp"
#include "gl/GLVertexArray.hpp"

namespace gnev {

template<IsVertex Attr, IsVertex Prim>
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
    GLBufferArrayT<GLuint> _prim_ibo;
    GLBufferArrayT<Prim> _prim_vbo;
    GLBufferVectorT<Attr> _attributes;

};

template<IsVertex Attr, IsVertex Prim>
Chunk<Attr, Prim>::Chunk(const std::shared_ptr<GladGLContext>& ctx,
                         const GLuint* primitive_indices, GLuint primitive_indices_count,
                         const Prim* primitive_vertices, GLuint primitive_vertices_count) :
    _vao(ctx),
    _prim_ibo(ctx, primitive_indices, primitive_indices_count, 0),
    _prim_vbo(ctx, primitive_vertices, primitive_vertices_count, 0),
    _attributes(ctx, GL_DYNAMIC_DRAW)
{
    _vao.glVertexArrayElementBuffer(_prim_ibo.handle());
    _vao.glVertexArrayVertexBuffer(0, _prim_vbo.handle(), 0, sizeof(Prim));
    _vao.glVertexArrayVertexBuffer(1, _attributes.handle(), 0, sizeof(Attr));
}   

template<IsVertex Attr, IsVertex Prim>
Chunk<Attr, Prim>::~Chunk()
{
}

template<IsVertex Attr, IsVertex Prim>
template<GLuint primitive_attribindex>
void Chunk<Attr, Prim>::bind_primitive(GLint shader_attribindex)
{
    static constexpr GLuint I = primitive_attribindex;
    static constexpr auto OFFSET = Prim::template get_offset<I>();
    static constexpr auto INFO = Prim::attrib_type<I>::info;

    _vao.glVertexArrayAttribBinding(shader_attribindex, 0);
    _vao.glVertexArrayAttribFormat(shader_attribindex, INFO.elements, INFO.type, INFO.normalized ? GL_TRUE : GL_FALSE, OFFSET);
    _vao.glEnableVertexArrayAttrib(shader_attribindex);

    std::cout << "bind_primitive " << I << " to " << shader_attribindex
              << " elements: " << INFO.elements
              << " type: " << INFO.type
              << " norm: " << (INFO.normalized ? "true" : "false")
              << std::endl;
}

template<IsVertex Attr, IsVertex Prim>
template<GLuint primitive_attribindex>
void Chunk<Attr, Prim>::bind_attrubute(GLint shader_attribindex)
{
    static constexpr GLuint I = primitive_attribindex;
    static constexpr auto OFFSET = Prim::template get_offset<I>();
    static constexpr auto INFO = Prim::attrib_type<I>::info;

    _vao.glVertexArrayAttribBinding(shader_attribindex, 1);
    _vao.glVertexArrayAttribFormat(shader_attribindex, INFO.elements, INFO.type, INFO.normalized, OFFSET);
    _vao.glEnableVertexArrayAttrib(shader_attribindex);
}



}