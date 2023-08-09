#pragma once

#include <iostream>
#include <unordered_map>
#include <queue>

#include "data/Vertex.hpp"
#include "gl/GLBufferVectorT.hpp"
#include "gl/GLVertexArray.hpp"

namespace gnev {

template<typename K, IsVertex V>
class TriangleMapT {
public:
    TriangleMapT(const std::shared_ptr<GladGLContext> &ctx);
    virtual ~TriangleMapT();

    template<size_t vertex_attrib>
    void bind(GLint attrib_index);
    void draw();

    void set(const K& key, const std::array<V, 3>& triangle);
    void set(const K& key, const V& v0, const V& v1, , const V& v2);
    void erase(const K& key);

private:
    GLVertexArray _vao;
    GLBufferVectorT<std::array<GLuint, 3>> _indices;
    GLBufferVectorT<std::array<V, 3>> _vertices;

    std::queue<GLuint> _unused_poses;
    std::unordered_map<K, GLuint> _pos_map;
    std::unordered_map<K, GLuint> _key_map;

    void _insert(const K& key, const std::array<V, 3>& triangle);
    void _assign(const auto& iter, const std::array<V, 3>& triangle);
};



template<typename K, IsVertex V>
TriangleMapT<K, V>::TriangleMapT(const std::shared_ptr<GladGLContext> &ctx)
    : _vao(ctx),
      _indices(ctx, nullptr, 0, GL_DYNAMIC_COPY),
      _vertices(ctx, nullptr, 0, GL_DYNAMIC_COPY)
{
    _vao.glVertexArrayElementBuffer(_indices.handle());
    _vao.glVertexArrayVertexBuffer(0, _vertices.handle(), 0, sizeof(V));
}

template<typename K, IsVertex V>
TriangleMapT<K, V>::~TriangleMapT()
{
}

template<typename K, IsVertex V>
template<size_t vertex_attrib>
void TriangleMapT<K, V>::bind(GLint attrib_index)
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

template<typename K, IsVertex V>
void TriangleMapT<K, V>::draw()
{
    _vao.glBindVertexArray();
    _vao.ctx()->DrawElements(GL_TRIANGLES, 3 * _indices.size(), GL_UNSIGNED_INT, 0);
}

template<typename K, IsVertex V>
void TriangleMapT<K, V>::set(const K& key, const std::array<V, 3>& triangle)
{
    auto found = _pos_map.find(key);
    if (found == _pos_map.end()){
        _insert(key, triangle);
    } else {
        _assign(found, triangle);
    }
}

template<typename K, IsVertex V>
void TriangleMapT<K, V>::_insert(const K& key, const std::array<V, 3>& triangle)
{
    GLuint pos;
    if (_unused_poses.empty()){
        pos = _indices.size();
    } else {
        pos = _unused_poses.back();
        _unused_poses.pop();
    }

    _pos_map.insert({key, pos});
    _key_map.insert({pos, key});
    _indices.push_back({3 * pos, 3 * pos + 1, 3 * pos + 2});
    _vertices.push_back(triangle);
}

template<typename K, IsVertex V>
void TriangleMapT<K, V>::_assign(const auto& iter, const std::array<V, 3>& triangle)
{
    GLuint pos = iter->second;
    _vertices.set(pos, triangle);
}

template<typename K, IsVertex V>
void TriangleMapT<K, V>::erase(const K& key)
{
    auto found = _pos_map.find(key);
    if (found == _pos_map.end()){
        throw std::runtime_error("Undefined key");
    }
    
    GLuint pos = _pos_map.extract(key);
    GLuint last_pos = _indices.size() - 1;
    K& last_key = _key_map.extract(last_pos);

    _pos_map[last_key] = pos;
    _key_map[pos] = last_key;

    _indices.copy_elements(pos, _indices.size() - 1, 1);
    _indices.pop_back();
    _vertices.copy_elements(pos, _vertices.size() - 1, 1);
    _vertices.pop_back();
}

}