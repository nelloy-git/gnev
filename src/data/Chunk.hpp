#pragma once

#include "data/Vertex.hpp"

#include "gl/GLBufferArrayT.hpp"
#include "gl/GLBufferVectorT.hpp"
#include "gl/GLVertexArray.hpp"

namespace gnev {

class Chunk {
    template<typename T>
    static constexpr bool is_index_type = std::is_same_v<T, GLubyte>
                                          || std::is_same_v<T, GLushort>
                                          || std::is_same_v<T, GLuint>;

    template<typename T>
    static constexpr GLenum gl_index_type = std::is_same_v<T, GLubyte> ? GL_UNSIGNED_BYTE 
                                            : std::is_same_v<T, GLushort> ? GL_UNSIGNED_SHORT
                                            : GL_UNSIGNED_INT;

public:
    template<typename IT, AttribInfo... PA, AttribInfo... IA,
             std::enable_if_t<is_index_type<IT>, bool> = true>
    Chunk(const std::shared_ptr<GladGLContext>& ctx,
          const GLBufferArrayT<IT>& primitive_indices,
          const GLBufferArrayT<Vertex<PA...>>& primitive_vertices,
          const GLBufferVectorT<Vertex<IA...>>& instance_vertices);
    virtual ~Chunk();

    void bind_primitive(GLint shader_attribindex, GLuint primitive_attrib);
    void bind_instance(GLint shader_attribindex, GLuint instance_attrib);

    void draw() const;

// private:
    GLVertexArray _vao;
    const GLenum _index_type;
    const GLenum _index_size;
    const std::vector<AttribInfo> _primitive_attrib_infos;
    const std::vector<AttribInfo> _instance_attrib_infos;
    GLBufferArray _primitive_indices;
    GLBufferArray _primitive_vertices;
    GLBufferVector _instaces;

    size_t get_primitive_attrib_offset(size_t attribindex) const;
    size_t get_instance_attrib_offset(size_t attribindex) const;
    static size_t get_index_type_size(GLenum index_type);

};

template<typename IT, AttribInfo... PA, AttribInfo... IA,
         std::enable_if_t<Chunk::is_index_type<IT>, bool>>
Chunk::Chunk(const std::shared_ptr<GladGLContext>& ctx,
             const GLBufferArrayT<IT>& primitive_indices,
             const GLBufferArrayT<Vertex<PA...>>& primitive_vertices,
             const GLBufferVectorT<Vertex<IA...>>& instance_vertices)
    : _vao(ctx),
      _index_type(gl_index_type<IT>),
      _index_size(get_index_type_size(gl_index_type<IT>)),
      _primitive_attrib_infos{PA...},
      _instance_attrib_infos{IA...},
      _primitive_indices(primitive_indices),
      _primitive_vertices(primitive_vertices),
      _instaces(instance_vertices)
{
    _vao.glVertexArrayElementBuffer(_primitive_indices.handle());
    _vao.glVertexArrayVertexBuffer(0, _primitive_vertices.handle(), 0, sizeof(Vertex<PA...>));
    _vao.glVertexArrayVertexBuffer(1, _instaces.handle(), 0, sizeof(Vertex<IA...>));
    _vao.glVertexArrayBindingDivisor(1, 1);
}   

Chunk::~Chunk()
{
}

void Chunk::bind_primitive(GLint shader_attribindex, GLuint primitive_attrib)
{
    auto& info = _primitive_attrib_infos[primitive_attrib];
    auto offset = get_primitive_attrib_offset(primitive_attrib);

    _vao.glVertexArrayAttribBinding(shader_attribindex, 0);
    _vao.glVertexArrayAttribFormat(shader_attribindex, info.elements, info.type, info.normalized ? GL_TRUE : GL_FALSE, offset);
    _vao.glEnableVertexArrayAttrib(shader_attribindex);

    std::cout << "Binded shader attrib " << shader_attribindex
              << " to primitive buffer attrib " << primitive_attrib
              << "; elements: " << info.elements
              << "; type: " << info.type
              << "; normalized: " << info.normalized
              << "; offset: " << offset
              << std::endl;
}

void Chunk::bind_instance(GLint shader_attribindex, GLuint instance_attrib)
{
    auto& info = _instance_attrib_infos[instance_attrib];
    auto offset = get_instance_attrib_offset(instance_attrib);

    _vao.glVertexArrayAttribBinding(shader_attribindex, 1);
    _vao.glVertexArrayAttribFormat(shader_attribindex, info.elements, info.type, info.normalized ? GL_TRUE : GL_FALSE, offset);
    _vao.glEnableVertexArrayAttrib(shader_attribindex);

    std::cout << "Binded shader attrib " << shader_attribindex
              << " to instance buffer attrib " << instance_attrib
              << "; elements: " << info.elements
              << "; type: " << info.type
              << "; normalized: " << info.normalized
              << "; offset: " << offset
              << std::endl;
}

void Chunk::draw() const
{
    _vao.glBindVertexArray();
    _vao.ctx()->PolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    _vao.ctx()->DrawElementsInstanced(GL_TRIANGLES, _primitive_indices.size() / _index_size, _index_type, 0, 2);
    _vao.ctx()->PolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

size_t Chunk::get_primitive_attrib_offset(size_t attribindex) const
{
    size_t offset = 0;
    for (size_t i = 0; i < attribindex; ++i){
        offset += _primitive_attrib_infos[i].size;
    }
    return offset;
}

size_t Chunk::get_instance_attrib_offset(size_t attribindex) const
{
    size_t offset = 0;
    for (size_t i = 0; i < attribindex; ++i){
        offset += _instance_attrib_infos[i].size;
    }
    return offset;
}

size_t Chunk::get_index_type_size(GLenum index_type)
{
    switch (index_type)
    {
    case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
    case GL_UNSIGNED_SHORT: return sizeof(GLubyte);
    case GL_UNSIGNED_INT: return sizeof(GLuint);
    
    default:
        throw std::runtime_error("");
    }
}

}