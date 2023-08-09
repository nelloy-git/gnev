#pragma once

#include "gl/GLBufferVectorCoherent.hpp"

namespace gnev {

template<typename T>
class EXPORT GLBufferVectorCoherentT : public GLBufferVectorCoherent {
public:
    using value_type = T;
    static constexpr GLuint value_size = sizeof(value_type);

    GLBufferVectorCoherentT(const std::shared_ptr<GladGLContext> &ctx, const T* initial_data, size_type initial_size, GLenum usage);
    virtual ~GLBufferVectorCoherentT();

    T& operator[](size_type i);
    const T& operator[](size_type i) const;

    GLuint size() const;
    GLuint capacity() const;

    T& at(size_type i);
    const T& at(size_type i) const;
    T* data();
    const T* data() const;

    bool empty() const;
    size_type size() const;
    void reserve(size_type capacity);
    size_type capacity() const;
    void shrink_to_fit();

    void clear();
    void insert(size_type i, const T& value);
    void insert_range(size_type first, size_type count, const T* values);
    void erase(size_type i);
    void erase_range(size_type first, size_type count);
    void push_back(const T& value);
    void push_back_range(size_type count, const T* values);
    void pop_back();
    void pop_back_range(size_type count);
};

template<typename T>
GLBufferVectorCoherentT<T>::GLBufferVectorCoherentT(const std::shared_ptr<GladGLContext> &ctx,
                                                    const T* initial_data, size_type initial_size, GLenum usage)
    : GLBufferVectorCoherent(ctx, static_cast<const void*>(initial_data), initial_size * value_size, usage)
{
}

template<typename T>
GLBufferVectorCoherentT<T>::~GLBufferVectorCoherentT()
{
}

template<typename T>
T& GLBufferVectorCoherentT<T>::operator[](size_type i)
{
    return at(i);
}

template<typename T>
const T& GLBufferVectorCoherentT<T>::operator[](size_type i) const
{
    return at(i);
}

template<typename T>
T& GLBufferVectorCoherentT<T>::at(size_type i)
{
    return *static_cast<T*>(GLBufferVectorCoherent::at(i * value_size));
}

template<typename T>
const T& GLBufferVectorCoherentT<T>::at(size_type i) const
{
    return *static_cast<const T*>(GLBufferVectorCoherent::at(i * value_size));
}

template<typename T>
T* GLBufferVectorCoherentT<T>::data()
{
    return static_cast<T*>(GLBufferVectorCoherent::data());
}

template<typename T>
const T* GLBufferVectorCoherentT<T>::data() const
{
    return static_cast<const T*>(GLBufferVectorCoherent::data());
}

template<typename T>
bool GLBufferVectorCoherentT<T>::empty() const
{
    return GLBufferVectorCoherent::empty();
}

template<typename T>
GLBufferVectorCoherentT<T>::size_type GLBufferVectorCoherentT<T>::size() const 
{
    return GLBufferVectorCoherent::size() / value_size;
}

template<typename T>
void GLBufferVectorCoherentT<T>::reserve(size_type capacity)
{
    GLBufferVectorCoherent::reserve(capacity * value_size);
}

template<typename T>
GLBufferVectorCoherentT<T>::size_type GLBufferVectorCoherentT<T>::capacity() const 
{
    return GLBufferVectorCoherent::capacity() / value_size;
}

template<typename T>
void GLBufferVectorCoherentT<T>::shrink_to_fit()
{
    return GLBufferVectorCoherent::shrink_to_fit();
}

template<typename T>
void GLBufferVectorCoherentT<T>::clear()
{
    return GLBufferVectorCoherent::clear();
}

template<typename T>
void GLBufferVectorCoherentT<T>::insert(size_type i, const T& value)
{
    GLBufferVectorCoherent::insert(i * value_size, value_size, static_cast<const void*>(&value));
}

template<typename T>
void GLBufferVectorCoherentT<T>::insert_range(size_type first, size_type count, const T* values)
{
    GLBufferVectorCoherent::insert(first * value_size, count * value_size, static_cast<const void*>(values));
}

template<typename T>
void GLBufferVectorCoherentT<T>::erase(size_type i)
{
    GLBufferVectorCoherent::remove(i * value_size, value_size);
}

template<typename T>
void GLBufferVectorCoherentT<T>::erase_range(size_type first, size_type count)
{
    GLBufferVectorCoherent::remove(first * value_size, count * value_size);
}

template<typename T>
void GLBufferVectorCoherentT<T>::push_back(const T& value)
{
    GLBufferVectorCoherent::push_back(value_size, static_cast<const void*>(&value));
}

template<typename T>
void GLBufferVectorCoherentT<T>::push_back_range(size_type count, const T* values)
{
    GLBufferVectorCoherent::push_back(count * value_size, static_cast<const void*>(values));
}

template<typename T>
void GLBufferVectorCoherentT<T>::pop_back()
{
    GLBufferVectorCoherent::pop_back(value_size);
}

template<typename T>
void GLBufferVectorCoherentT<T>::pop_back_range(size_type count)
{
    GLBufferVectorCoherent::pop_back(count * value_size);
}

}