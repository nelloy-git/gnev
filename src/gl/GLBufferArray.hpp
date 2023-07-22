#pragma once

#include <stdexcept>

#include "gl/GLBuffer.hpp"

namespace gnev {

template<typename T>
class EXPORT GLBufferArray : public GLBuffer {
public:
    static constexpr GLuint ELEM_SIZE = sizeof(T);

    GLBufferArray(const GLBuffer& buffer);
    GLBufferArray(const std::shared_ptr<GladGLContext> &ctx, GLuint length, GLbitfield storage_flags, const T* data = nullptr);
    ~GLBufferArray();

    GLuint size() const;

    T* map(GLenum access);
    T* map(GLuint first, GLuint length, GLbitfield access);
    void unmap();

    std::unique_ptr<T> get(GLuint i) const;
    void set(GLuint i, const T& value);

private:
    const GLuint _size = 0;
};

template<typename T>
GLBufferArray<T>::GLBufferArray(const GLBuffer& buffer) :
    GLBuffer(buffer)
{
    GLint allocated_bytes = 0;
    buffer.glGetBufferParameteriv(GL_BUFFER_SIZE, &allocated_bytes);
    _size = allocated_bytes / ELEM_SIZE;
}

template<typename T>
GLBufferArray<T>::GLBufferArray(const std::shared_ptr<GladGLContext> &ctx, GLuint length, GLbitfield storage_flags, const T* data) :
    GLBuffer(ctx),
    _size(length)
{
    glBufferStorage(length * ELEM_SIZE, data, storage_flags);
}

template<typename T>
GLBufferArray<T>::~GLBufferArray()
{
}

template<typename T>
GLuint GLBufferArray<T>::size() const 
{
    return _size;
}

template<typename T>
T* GLBufferArray<T>::map(GLenum access)
{
    return glMapBuffer(access);
}

template<typename T>
T* GLBufferArray<T>::map(GLuint first, GLuint length, GLbitfield access)
{
    if (first + length >= _size){
        throw std::out_of_range("");
    }
    return glMapBufferRange(first * ELEM_SIZE, length * ELEM_SIZE, access);
}

template<typename T>
void GLBufferArray<T>::unmap()
{
    glUnmapBuffer();
}

template<typename T>
std::unique_ptr<T> GLBufferArray<T>::get(GLuint i) const
{
    if (i >= _size){
        throw std::out_of_range("");
    }

    void* data = malloc(ELEM_SIZE);
    glGetBufferSubData(i * ELEM_SIZE, ELEM_SIZE, data);
    return std::unique_ptr<T>(static_cast<T*>(data));
}

template<typename T>
void GLBufferArray<T>::set(GLuint i, const T& value)
{
    if (i >= _size){
        throw std::out_of_range("");
    }
    glBufferSubData(i * ELEM_SIZE, ELEM_SIZE, &value);
}

}