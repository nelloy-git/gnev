#pragma once

#include "gl/GLBufferArray.hpp"

namespace gnev {

template<typename T>
class EXPORT GLBufferArrayT : public GLBufferArray {
public:
    static constexpr GLuint ELEM_SIZE = sizeof(T);

    GLBufferArrayT(const GLBufferArrayT& untyped_vector);
    GLBufferArrayT(const std::shared_ptr<GladGLContext> &ctx, const T* initial_data, GLsizeiptr initial_size, GLbitfield storage_flags);
    ~GLBufferArrayT();

    GLuint size() const;

    T* glMapBuffer(GLenum access);
    T* glMapBufferRange(GLuint first, GLuint length, GLbitfield access);

    std::shared_ptr<T> get(GLuint i) const;
    void set(GLuint i, const T& value);
};

template<typename T>
GLBufferArrayT<T>::GLBufferArrayT(const GLBufferArrayT& untyped_vector) :
    GLBufferArray(untyped_vector)
{
}

template<typename T>
GLBufferArrayT<T>::GLBufferArrayT(const std::shared_ptr<GladGLContext> &ctx, const T* initial_data, GLsizeiptr initial_size, GLbitfield storage_flags) :
    GLBufferArray(ctx, static_cast<const void*>(initial_data), initial_size * ELEM_SIZE, storage_flags)
{
}

template<typename T>
GLBufferArrayT<T>::~GLBufferArrayT()
{
}

template<typename T>
GLuint GLBufferArrayT<T>::size() const 
{
    return GLBufferArray::size() / ELEM_SIZE;
}

template<typename T>
T* GLBufferArrayT<T>::glMapBuffer(GLenum access)
{
    return glMapBuffer(access);
}

template<typename T>
T* GLBufferArrayT<T>::glMapBufferRange(GLuint first, GLuint length, GLbitfield access)
{
    if (first + length >= _size){
        throw std::out_of_range("");
    }
    return glMapBufferRange(first * ELEM_SIZE, length * ELEM_SIZE, access);
}

template<typename T>
std::shared_ptr<T> GLBufferArrayT<T>::get(GLuint i) const
{
    return std::static_pointer_cast<T>(GLBufferArray::get(i * ELEM_SIZE, ELEM_SIZE));
}

template<typename T>
void GLBufferArrayT<T>::set(GLuint i, const T& value)
{
    GLBufferVector::set(i * ELEM_SIZE, ELEM_SIZE, static_cast<const void*>(&value));
}

}