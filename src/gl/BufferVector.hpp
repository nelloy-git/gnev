#pragma once

#include <iostream>

#include "gl/Buffer.hpp"

namespace gnev::gl {

template<typename T>
class EXPORT BufferVector : public Buffer {
public:
    using type = T;
    static constexpr GLsizeiptr base_cap = 4;

    BufferVector(const GladCtx& ctx, GLsizeiptr initial_size, const T* initial_data);
    virtual ~BufferVector();

    std::unique_ptr<T, void(*)(T*)> get(GLsizeiptr i) const;
    void set(GLsizeiptr i, const T& value);

    std::unique_ptr<T[], void(*)(T*)> get_range(GLsizeiptr i, GLsizeiptr count) const;
    void set_range(GLsizeiptr i, GLsizeiptr count, const T* data);

    GLsizeiptr size() const;
    void reserve(GLsizeiptr capacity);
    GLsizeiptr capacity() const;
    void shrink_to_fit();
    
    void clear();
    void insert(GLsizeiptr i, const T& value);
    void insert_range(GLsizeiptr i, const T* value, GLuint count);
    void erase(GLsizeiptr i);
    void erase_range(GLsizeiptr i, GLuint count);
    void push_back(const T& value);
    void push_back_range(const T* values, GLuint count);
    void pop_back();
    void pop_back_range(GLuint count);

    void set_resize_params(float mult, float add);

private:
    struct SharedPrivate {
        GLsizeiptr buffer_size = 0;
        GLsizeiptr buffer_cap = 0;
        double cap_mult = 2;
        double cap_add = 0;
    };
    std::shared_ptr<SharedPrivate> _priv;

    static constexpr GLenum _usage = GL_DYNAMIC_COPY;
    static T* _alloc(GLsizeiptr n);
    static void _free(T* ptr);
};



template<typename T>
BufferVector<T>::BufferVector(const GladCtx& ctx, GLsizeiptr initial_size, const T* initial_data)
    : Buffer(ctx),
      _priv(std::make_shared<SharedPrivate>())
{
    _priv->buffer_size = initial_size * sizeof(T);
    _priv->buffer_cap = std::max(initial_size * sizeof(T), base_cap * sizeof(T));
    glBufferData(_priv->buffer_cap, initial_data, _usage);
}

template<typename T>
BufferVector<T>::~BufferVector()
{
}

template<typename T>
std::unique_ptr<T, void(*)(T*)> BufferVector<T>::get(GLsizeiptr i) const
{
    if (i < 0 || i >= size()){throw std::out_of_range("");}

    std::unique_ptr<T, void(*)(T*)> data(_alloc(1), &_free);
    glGetBufferSubData(i * sizeof(T), sizeof(T), data.get());
    return data;
}

template<typename T>
void BufferVector<T>::set(GLsizeiptr i, const T& value)
{
    if (i < 0 || i >= size()){throw std::out_of_range("");}
    glBufferSubData(i * sizeof(T), sizeof(T), &value);
}

template<typename T>
std::unique_ptr<T[], void(*)(T*)> BufferVector<T>::get_range(GLsizeiptr i, GLsizeiptr count) const
{
    if (i < 0 || i + count >= size()){throw std::out_of_range("");}
    std::unique_ptr<T[], void(*)(T*)> data(_alloc(count), &_free);
    glGetBufferSubData(i * sizeof(T), count * sizeof(T), data.get());
    return data;
}

template<typename T>
void BufferVector<T>::set_range(GLsizeiptr i, GLsizeiptr count, const T* data)
{
    if (i < 0 || i + count >= size()){throw std::out_of_range("");}
    glBufferSubData(i * sizeof(T), count * sizeof(T), data);
}

template<typename T>
GLsizeiptr BufferVector<T>::size() const
{
    return _priv->buffer_size / sizeof(T);
}

template<typename T>
void BufferVector<T>::reserve(GLsizeiptr cap)
{
    if (cap < 0){throw std::out_of_range("");}

    if (capacity() >= cap){
        return;
    }

    _priv->buffer_cap = cap * sizeof(T);
    if (_priv->buffer_size == 0){
        glBufferData(cap * sizeof(T), nullptr, _usage);
        return;
    }

    Buffer tmp_buffer(ctx());
    tmp_buffer.glBufferStorage(_priv->buffer_size, nullptr, 0);
    glCopyBufferSubData(tmp_buffer.handle(), 0, 0, _priv->buffer_size);

    glBufferData(_priv->buffer_cap, nullptr, _usage);
    tmp_buffer.glCopyBufferSubData(handle(), 0, 0, _priv->buffer_size);
}

template<typename T>
GLsizeiptr BufferVector<T>::capacity() const
{
    return _priv->buffer_cap / sizeof(T);
}

template<typename T>
void BufferVector<T>::shrink_to_fit()
{
    _priv->buffer_cap = _priv->buffer_size;
    if (_priv->buffer_size == 0){
        glBufferData(sizeof(T), nullptr, _usage);
        return;
    }

    Buffer tmp_buffer(ctx());
    tmp_buffer.glBufferStorage(_priv->buffer_size, nullptr, 0);
    glCopyBufferSubData(tmp_buffer.handle(), 0, 0, _priv->buffer_size);
    
    glBufferData(_priv->buffer_size, nullptr, _usage);
    tmp_buffer.glCopyBufferSubData(handle(), 0, 0, _priv->buffer_size);
}

template<typename T>
void BufferVector<T>::clear()
{
    _priv->buffer_size = 0;
}

template<typename T>
void BufferVector<T>::insert(GLsizeiptr i, const T& value) 
{
    insert_range(i, &value, 1);
}

template<typename T>
void BufferVector<T>::insert_range(GLsizeiptr i, const T* value, GLuint count) 
{
    if (i < 0 || i > size()){throw std::out_of_range("");}

    while (size() + count > capacity()){
        reserve(capacity() * _priv->cap_mult + _priv->cap_add);
    }

    if (i < size() && size() != 0){
        glCopyBufferSubData(handle(), i * sizeof(T), (i+count) * sizeof(T), (size() - i) * sizeof(T));
    }
    glBufferSubData(i * sizeof(T), count * sizeof(T), value);
    _priv->buffer_size += count * sizeof(T);
}

template<typename T>
void BufferVector<T>::erase(GLsizeiptr i)
{
    erase_range(i, 1);
}

template<typename T>
void BufferVector<T>::erase_range(GLsizeiptr i, GLuint count)
{
    if (i < 0 || i + count > size()){throw std::out_of_range("");}

    if (i + count < size()){
        glCopyBufferSubData(handle(), (i+count) * sizeof(T), (i) * sizeof(T), (size() - i - count) * sizeof(T));
    }
    _priv->buffer_size -= count * sizeof(T);
}

template<typename T>
void BufferVector<T>::push_back(const T& value)
{
    insert(size(), value);
}

template<typename T>
void BufferVector<T>::push_back_range(const T* values, GLuint count)
{
    insert_range(size(), values, count);
}

template<typename T>
void BufferVector<T>::pop_back()
{
    erase(size() - 1);
}

template<typename T>
void BufferVector<T>::pop_back_range(GLuint count)
{
    erase_range(size() - count, count);
}

template<typename T>
T* BufferVector<T>::_alloc(GLsizeiptr n)
{
    return static_cast<T*>(malloc(n * sizeof(T)));
}

template<typename T>
void BufferVector<T>::_free(T* ptr)
{
    free(ptr);
}


}