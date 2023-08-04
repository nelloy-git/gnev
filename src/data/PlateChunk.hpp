#pragma once

#include <iostream>
#include <unordered_map>
#include <queue>

#include "data/Vertex.hpp"
#include "gl/GLBufferVectorT.hpp"
#include "gl/GLVertexArray.hpp"

namespace gnev {

template<IsVertex V>
class PlateChunk {
public:
    PlateChunk(const std::shared_ptr<GladGLContext> &ctx);
    virtual ~PlateChunk();

    GLuint add(const std::array<V, 4>& v);
    void remove(GLuint id);

    template<size_t vertex_attrib>
    void bind(GLint attrib_index);
    void draw() const;

    GLVertexArray _vao;
    GLBufferVectorT<std::array<GLuint, 6>> _indices;
    GLBufferVectorT<std::array<V, 4>> _vertices;

    std::queue<GLuint> _unused_vertices;
    GLuint _cur_id = 0;
    std::unordered_map<GLuint, std::pair<GLuint, GLuint>> _block_pos;
};

template<IsVertex V>
PlateChunk<V>::PlateChunk(const std::shared_ptr<GladGLContext> &ctx)
    : _vao(ctx),
      _indices(ctx, nullptr, 0, GL_DYNAMIC_COPY),
      _vertices(ctx, nullptr, 0, GL_DYNAMIC_COPY)
{
    _vao.glVertexArrayElementBuffer(_indices.handle());
    _vao.glVertexArrayVertexBuffer(0, _vertices.handle(), 0, sizeof(V));
}

template<IsVertex V>
PlateChunk<V>::~PlateChunk()
{
}

template<IsVertex V>
GLuint PlateChunk<V>::add(const std::array<V, 4>& v)
{
    GLuint vert_i;
    if (!_unused_vertices.empty()){
        vert_i = _unused_vertices.back();
        _unused_vertices.pop();
        _vertices.set(vert_i, v);
    } else {
        vert_i = _vertices.size();
        _vertices.push_back(v);
    }
    
    GLuint id = _cur_id++;
    _block_pos[id] = {_indices.size(), vert_i};

    auto base = 6 * vert_i;
    _indices.push_back({base, base + 1, base + 2, base, base + 2, base + 3});
    return id;
}

template<IsVertex V>
void PlateChunk<V>::remove(GLuint id)
{
    auto pos = _block_pos[id];
    auto index_pos = pos.first;
    auto vert_pos = pos.second;

    _indices.remove(index_pos);
    _unused_vertices.push(vert_pos);
}

template<IsVertex V>
template<size_t vertex_attrib>
void PlateChunk<V>::bind(GLint attrib_index)
{
    constexpr auto INFO = V::attrib_type<vertex_attrib>::info;
    constexpr auto OFFSET = V::template get_offset<vertex_attrib>();

    _vao.glVertexArrayAttribBinding(attrib_index, 0);
    _vao.glVertexArrayAttribFormat(attrib_index, INFO.elements, INFO.type, INFO.normalized ? GL_TRUE : GL_FALSE, OFFSET);
    _vao.glEnableVertexArrayAttrib(attrib_index);

    std::cout << "Binded shader attrib " << attrib_index
              << " to primitive buffer attrib " << vertex_attrib
              << "; elements: " << INFO.elements
              << "; type: " << INFO.type
              << "; normalized: " << INFO.normalized
              << "; offset: " << OFFSET
              << std::endl;
}

template<IsVertex V>
void PlateChunk<V>::draw() const
{
    _vao.glBindVertexArray();
    _vao.ctx()->DrawElements(GL_TRIANGLES, 6 * _indices.size(), GL_UNSIGNED_INT, 0);
}

}