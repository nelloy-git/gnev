#pragma once

#include <stdexcept>
#include <memory>

#include "gl/GLBuffer.hpp"
#include "util/Util.hpp"

namespace gnev {

template<typename T>
class EXPORT GLVectorBuffer final {
public:
    static constexpr GLsizei CAPACITY_BASE = 4;
    static constexpr GLuint CAPACITY_MULT = 2;
    static GLint MAX_CAPACITY(GladGLContext& ctx);

    GLVectorBuffer(const std::shared_ptr<GladGLContext> &ctx);
    ~GLVectorBuffer(){};

    std::unique_ptr<T> get(GLuint i) const;
    void set(GLuint i, const T& value);
    void push_back(const T& value);
    void shape();
    void reserve(GLuint capacity);

    GLBuffer& buffer(){return *_buffer;}
    const GLBuffer& buffer() const {return *_buffer;}
    GLuint size() const {return _size;}
    GLuint capacity() const {return _capacity;}

private:
    static constexpr GLuint _ELEM_SIZE = sizeof(T);
    static constexpr GLint _FLAGS = GL_DYNAMIC_STORAGE_BIT;

    const std::unique_ptr<GLBuffer> _buffer;

    GLuint _size = 0;
    GLuint _capacity = CAPACITY_BASE;
};





template<typename T>
GLVectorBuffer<T>::GLVectorBuffer(const std::shared_ptr<GladGLContext> &ctx) :
    _buffer(std::make_unique<GLBuffer>(ctx)){
    _buffer->glBufferStorage(_ELEM_SIZE * CAPACITY_BASE, nullptr, _FLAGS);
}

template<typename T>
std::unique_ptr<T> GLVectorBuffer<T>::get(GLuint i) const {
    if (i >= _size){
        throw std::out_of_range();
    }

    void* data = malloc(_ELEM_SIZE);
    _buffer->glGetBufferSubData(i * _ELEM_SIZE, _ELEM_SIZE, data);
    return std::unique_ptr<T>(static_cast<T*>(data));
}

template<typename T>
void GLVectorBuffer<T>::set(GLuint i, const T& value){
    if (i >= _size){
        throw std::out_of_range();
    }
    _buffer->glBufferSubData(i * _ELEM_SIZE, _ELEM_SIZE, &value);
}

template<typename T>
void GLVectorBuffer<T>::push_back(const T& value){
    if (_size >= _capacity){
        reserve(_capacity * CAPACITY_MULT);
    }
    ++_size;
    set(_size, value);
}

template<typename T>
void GLVectorBuffer<T>::shape(){
    auto new_buffer = std::make_unique<GLBuffer>(_buffer->ctx);
    new_buffer->glBufferStorage(_ELEM_SIZE * _size, nullptr, _FLAGS);
    _buffer->glCopyBufferSubData(new_buffer->handle, 0, 0, _ELEM_SIZE * _size);
    _buffer = std::move(new_buffer);
}

template<typename T>
void GLVectorBuffer<T>::reserve(GLuint capacity){
    if (_capacity >= capacity){
        return;
    }

    _capacity = capacity;
    auto new_buffer = std::make_unique<GLBuffer>(_buffer->ctx);
    new_buffer->glBufferStorage(_ELEM_SIZE * _capacity, nullptr, _FLAGS);
    _buffer->glCopyBufferSubData(new_buffer->handle, 0, 0, _ELEM_SIZE * _capacity);
    _buffer = std::move(new_buffer);
}

}