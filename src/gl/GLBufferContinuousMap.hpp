#pragma once

#include "bimap.hpp"

#include "gl/GLBuffer.hpp"

namespace gnev {

template<typename K>
class EXPORT GLBufferContinuousMap final {
public:
    GLBufferContinuousMap(const std::shared_ptr<GladGLContext> &ctx, GLuint elem_size,
                          GLuint cap_base = 1, GLuint cap_mult = 2, GLuint cap_add = 0);
    virtual ~GLBufferContinuousMap();

    std::unique_ptr<void> get(const K& key) const;
    void set(const K& key, const void* data);
    void erase(const K& key);
                          
    void shape();
    void reserve(GLuint cap);

    GLBuffer& buffer();
    const GLBuffer& buffer() const;

private:
    static constexpr GLint _FLAGS = GL_DYNAMIC_STORAGE_BIT;

    const GLuint _elem_size;
    const GLuint _cap_mult;
    const GLuint _cap_add;

    GLuint _size;
    GLuint _cap;
    GLBuffer _buffer;
    stde::unordered_bimap<K, GLuint> _pos_map;
};

template<typename K>
GLBufferContinuousMap<K>::GLBufferContinuousMap(const std::shared_ptr<GladGLContext> &ctx, GLuint elem_size,
                                                      GLuint cap_base, GLuint cap_mult, GLuint cap_add) :
    _elem_size(elem_size),
    _cap_mult(cap_mult),
    _cap_add(cap_add),
    _size(0),
    _cap(cap_base),
    _buffer(ctx)
{
    _buffer.glBufferStorage(cap_base * elem_size, nullptr, _FLAGS);
}

template<typename K>
GLBufferContinuousMap<K>::~GLBufferContinuousMap()
{
}

template<typename K>
std::unique_ptr<void> GLBufferContinuousMap<K>::get(const K& key) const
{
    auto buffer_pos = _pos_map.get_value(key);

    void *data = malloc(_elem_size);
    _buffer.glGetBufferSubData(buffer_pos * _elem_size, _elem_size, data);
    return std::unique_ptr<void>(data);
}

template<typename K>
void GLBufferContinuousMap<K>::set(const K& key, const void* data)
{
    // Override existing value
    if (_pos_map.has_key(key)){
        GLuint buffer_pos = _pos_map.get_value(key);
        _buffer.glBufferSubData(buffer_pos, _elem_size, data);
        return;
    }

    if (_size >= _cap){
        reserve(_cap * _cap_mult + _cap_add);
    }
    GLuint buffer_pos = _size;
    ++_size;
    _buffer.glBufferSubData(buffer_pos * _elem_size, _elem_size, data);
    _pos_map.insert(key, buffer_pos);
}

template<typename K>
void GLBufferContinuousMap<K>::erase(const K& key)
{
    GLuint buffer_pos = _pos_map.erase_key(key);
    --_size;

    if (buffer_pos == _size){
        return;
    }

    // Move last buffer element to removed pos
    _buffer.glCopyBufferSubData(_buffer.handle(), _size * _elem_size, buffer_pos * _elem_size, _elem_size);
    auto moved_key = _pos_map.get_value(_size);
    _pos_map.erase_value(_size);
    _pos_map.insert(key, buffer_pos);
}

template<typename K>
void GLBufferContinuousMap<K>::shape()
{
    GLBuffer new_buffer(_buffer.ctx());
    new_buffer.glBufferStorage(_size * _elem_size, nullptr, _FLAGS);
    _buffer.glCopyBufferSubData(new_buffer.handle(), 0, 0, _size * _elem_size);
    _buffer = new_buffer;
}

template<typename K>
void GLBufferContinuousMap<K>::reserve(GLuint capacity)
{
    if (_cap >= capacity){
        return;
    }

    _cap = capacity;
    GLBuffer new_buffer(_buffer.ctx());
    new_buffer.glBufferStorage(_cap * _elem_size, nullptr, _FLAGS);
    _buffer.glCopyBufferSubData(new_buffer.handle(), 0, 0, _size * _elem_size);
    _buffer = new_buffer;
}

template<typename K>
GLBuffer& GLBufferContinuousMap<K>::buffer()
{
    return _buffer;
}

template<typename K>
const GLBuffer& GLBufferContinuousMap<K>::buffer() const
{
    return _buffer;
}

}