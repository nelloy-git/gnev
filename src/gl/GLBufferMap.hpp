#pragma once

#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

#include "gl/GLBuffer.hpp"

#ifndef GNEV_GL_BUFFER_MAP_CAP_BASE
#define GNEV_GL_BUFFER_MAP_CAP_BASE 1
#endif

#ifndef GNEV_GL_BUFFER_MAP_CAP_MULT
#define GNEV_GL_BUFFER_MAP_CAP_MULT 2
#endif

#ifndef GNEV_GL_BUFFER_MAP_CAP_ADD
#define GNEV_GL_BUFFER_MAP_CAP_ADD 0
#endif

namespace gnev {

template<typename K>
class EXPORT GLBufferMap final {
public:
    static constexpr GLsizei CAPACITY_BASE = 1;

    GLBufferMap(const std::shared_ptr<GladGLContext> &ctx, GLuint elem_size, GLuint capacity_mult = 2, GLuint capacity_add = 0);
    ~GLBufferMap(){};

    std::unique_ptr<void> get(const K& key) const;
    void set(const K& key, const void* data);
    void remove(const K&);

    void shape();
    void reserve(GLuint capacity);

    GLBuffer& buffer(){return _buffer;}
    const GLBuffer& buffer() const {return _buffer;}
    GLuint size() const {return _size;}

private:
    static constexpr GLint _FLAGS = GL_DYNAMIC_STORAGE_BIT;

    const GLuint _elem_size;
    const GLuint _capacity_mult;
    const GLuint _capacity_add;

    GLBuffer _buffer;
    std::unordered_map<K, GLuint> _pos_map;
    std::unordered_set<GLuint> _unused_poses;

    GLuint _capacity = CAPACITY_BASE;
};

template<typename K>
GLBufferMap<K>::GLBufferMap(const std::shared_ptr<GladGLContext> &ctx, GLuint elem_size, GLuint capacity_mult, GLuint capacity_add) :
    _elem_size(elem_size),
    _capacity_mult(capacity_mult),
    _capacity_add(capacity_add),
    _buffer(ctx){
    _buffer.glBufferStorage(elem_size * CAPACITY_BASE, nullptr, _FLAGS);
    _unused_poses.insert(0);
}

template<typename K>
GLBufferMap<K>::~GLBufferMap(){
}

template<typename K>
std::unique_ptr<void> GLBufferMap<K>::get(const K& key) const {
    auto buffer_pos = _pos_map.at(key);

    void *data = malloc(_elem_size);
    _buffer.glGetBufferSubData(buffer_pos * _elem_size, _elem_size, data);
    return std::unique_ptr<void>(data);
}

template<typename K>
void GLBufferMap<K>::set(const K& key, const void* data){
    GLuint buffer_pos;
    auto it = _pos_map.find(key);
    if (it == _pos_map.end()){
        if (_unused_poses.begin() == _unused_poses.end()){
            reserve(_capacity * _capacity_mult + _capacity_add);
        }

        buffer_pos = _unused_poses.extract(_unused_poses.begin());
        _pos_map[key] = buffer_pos;
    }
    _buffer.glBufferSubData(buffer_pos, _elem_size, data);
}

template<typename K>
void GLBufferMap<K>::remove(const K& key){
    auto buffer_pos = _pos_map.extract(key);
    _unused_poses.insert(buffer_pos);
}

template<typename K>
void GLBufferMap<K>::shape(){
    GLBuffer new_buffer(_buffer.ctx());
    new_buffer.glBufferStorage(_elem_size * _pos_map.size(), nullptr, _FLAGS);

    _unused_poses.clear();

    GLuint i = 0;
    for (auto& it : _pos_map){
        auto buffer_pos = it.second;
        _buffer(new_buffer.handle(), buffer_pos * _elem_size, i * _elem_size, _elem_size);
        *it = i++;
    }
}

template<typename K>
void GLBufferMap<K>::reserve(GLuint capacity){
    if (_capacity >= capacity){
        return;
    }

    for (GLuint i = _capacity; i < capacity; ++i){
        _unused_poses.insert(i);
    }

    _capacity = capacity;
    GLBuffer new_buffer(_buffer.ctx());
    new_buffer.glBufferStorage(_elem_size * _capacity, nullptr, _FLAGS);
    _buffer.glCopyBufferSubData(new_buffer.handle(), 0, 0, _elem_size * _capacity);
    _buffer = new_buffer;
}

}