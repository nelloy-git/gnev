#pragma once

#include "gl/GLBufferVector.hpp"

namespace gnev {

template<typename T>
class EXPORT GLBufferVectorT : public GLBufferVector {
public:
    static constexpr GLuint ELEM_SIZE = sizeof(T);

    GLBufferVectorT(const GLBufferVector&) = delete;
    GLBufferVectorT(GLBufferVector&& untyped_vector);
    GLBufferVectorT(const std::shared_ptr<GladGLContext> &ctx, const T* initial_data, GLsizeiptr initial_size, GLenum usage);
    virtual ~GLBufferVectorT();

    GLuint size() const;
    GLuint capacity() const;

    T* glMapBuffer(GLenum access);
    T* glMapBufferRange(GLuint first, GLuint length, GLbitfield access);

    std::shared_ptr<T> get(GLuint i) const;
    void set(GLuint i, const T& value);
    void push_back(const T& value);
    void insert(GLuint i, const T& value);
    void remove(GLuint i);
    void reserve(GLuint capacity);

};

template<typename T>
GLBufferVectorT<T>::GLBufferVectorT(GLBufferVector&& untyped_vector)
    : GLBufferVector(untyped_vector)
{
}

template<typename T>
GLBufferVectorT<T>::GLBufferVectorT(const std::shared_ptr<GladGLContext> &ctx, const T* initial_data, GLsizeiptr initial_size, GLenum usage)
    : GLBufferVector(ctx, static_cast<const void*>(initial_data), initial_size * ELEM_SIZE, usage)
{
}

template<typename T>
GLBufferVectorT<T>::~GLBufferVectorT()
{
}

template<typename T>
GLuint GLBufferVectorT<T>::size() const 
{
    return GLBufferVector::size() / ELEM_SIZE;
}

template<typename T>
GLuint GLBufferVectorT<T>::capacity() const 
{
    return GLBufferVector::capacity() / ELEM_SIZE;
}

template<typename T>
T* GLBufferVectorT<T>::glMapBuffer(GLenum access)
{
    return GLBufferVector::glMapBuffer(access);
}

template<typename T>
T* GLBufferVectorT<T>::glMapBufferRange(GLuint first, GLuint size, GLbitfield access)
{
    if (first + size >= _size){
        throw std::out_of_range("");
    }
    return glMapBufferRange(first * ELEM_SIZE, size * ELEM_SIZE, access);
}

template<typename T>
std::shared_ptr<T> GLBufferVectorT<T>::get(GLuint i) const
{
    return std::static_pointer_cast<T>(GLBufferVector::get(i * ELEM_SIZE, ELEM_SIZE));
}

template<typename T>
void GLBufferVectorT<T>::set(GLuint i, const T& value)
{
    GLBufferVector::set(i * ELEM_SIZE, ELEM_SIZE, static_cast<const void*>(&value));
}

template<typename T>
void GLBufferVectorT<T>::push_back(const T& value)
{
    GLBufferVector::push_back(ELEM_SIZE, static_cast<const void*>(&value));
}

template<typename T>
void GLBufferVectorT<T>::insert(GLuint i, const T& value)
{
    GLBufferVector::insert(i * ELEM_SIZE, ELEM_SIZE, static_cast<const void*>(&value));
}

template<typename T>
void GLBufferVectorT<T>::remove(GLuint i)
{
    GLBufferVector::remove(i * ELEM_SIZE, ELEM_SIZE);
}

template<typename T>
void GLBufferVectorT<T>::reserve(GLuint capacity)
{
    GLBufferVector::reserve(capacity * ELEM_SIZE);
}

}