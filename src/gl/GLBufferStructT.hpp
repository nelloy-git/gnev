#pragma once

#include "gl/GLBufferArray.hpp"

namespace gnev {

template<typename T>
class EXPORT GLBufferStructT : public GLBufferArray {
public:
    static constexpr GLuint ELEM_SIZE = sizeof(T);

    GLBufferStructT(const GLBufferStructT& untyped_vector);
    GLBufferStructT(const std::shared_ptr<GladGLContext> &ctx, const T* initial_data, GLbitfield storage_flags);
    ~GLBufferStructT();

    T* glMapBuffer(GLenum access);

    std::shared_ptr<T> get() const;
    void set(const T& value);
};

template<typename T>
GLBufferStructT<T>::GLBufferStructT(const GLBufferStructT& untyped_vector) :
    GLBufferArray(untyped_vector)
{
}

template<typename T>
GLBufferStructT<T>::GLBufferStructT(const std::shared_ptr<GladGLContext> &ctx, const T* initial_data, GLbitfield storage_flags) :
    GLBufferArray(ctx, static_cast<const void*>(initial_data), ELEM_SIZE, storage_flags)
{
}

template<typename T>
GLBufferStructT<T>::~GLBufferStructT()
{
}

template<typename T>
T* GLBufferStructT<T>::glMapBuffer(GLenum access)
{
    return static_cast<T*>(GLBufferArray::glMapBuffer(access));
}

template<typename T>
std::shared_ptr<T> GLBufferStructT<T>::get() const
{
    return std::static_pointer_cast<T>(GLBufferArray::get(0, ELEM_SIZE));
}

template<typename T>
void GLBufferStructT<T>::set(const T& value)
{
    GLBufferArray::set(0, ELEM_SIZE, static_cast<const void*>(&value));
}

}