#pragma once

#include <stdexcept>

#include "gl/Buffer.hpp"

namespace gnev::gl {

template<typename T>
class EXPORT BufferArray : public Buffer {
public:
    using type = T;

    BufferArray(const GladCtx& ctx, GLsizeiptr size, const T* data, GLbitfield storage_flags);
    virtual ~BufferArray();

    GLsizeiptr size() const;

    std::unique_ptr<T, void(*)(T*)> get(GLsizeiptr i) const;
    void set(GLsizeiptr i, const T& value);

    std::unique_ptr<T[], void(*)(T*)> get_range(GLsizeiptr i, GLsizeiptr count) const;
    void set_range(GLsizeiptr i, GLsizeiptr count, const T* data);

private:
    GLsizeiptr _size;

    static T* _alloc(GLsizeiptr n);
    static void _free(T* ptr);
};

template<typename T>
BufferArray<T>::BufferArray(const GladCtx& ctx, GLsizeiptr size, const T* data, GLbitfield storage_flags)
    : Buffer(ctx),
      _size(size)
{
    glBufferStorage(size * sizeof(T), data, storage_flags);
}

template<typename T>
BufferArray<T>::~BufferArray()
{
}

template<typename T>
GLsizeiptr BufferArray<T>::size() const
{
    return _size;
}

template<typename T>
std::unique_ptr<T, void(*)(T*)> BufferArray<T>::get(GLsizeiptr i) const
{
    if (i < 0 || i >= _size){throw std::out_of_range("");}

    std::unique_ptr<T, void(*)(T*)> data(_alloc(1), &_free);
    glGetBufferSubData(i, sizeof(T), data.get());
    return data;
}

template<typename T>
void BufferArray<T>::set(GLsizeiptr i, const T& value)
{
    if (i < 0 || i >= _size){throw std::out_of_range("");}
    glBufferSubData(i, sizeof(T), &value);
}

template<typename T>
std::unique_ptr<T[], void(*)(T*)> BufferArray<T>::get_range(GLsizeiptr i, GLsizeiptr count) const
{
    if (i < 0 || i + count >= _size){throw std::out_of_range("");}
    std::unique_ptr<T[], void(*)(T*)> data(_alloc(count), &_free);
    glGetBufferSubData(i, count, data.get());
    return data;
}

template<typename T>
void BufferArray<T>::set_range(GLsizeiptr i, GLsizeiptr count, const T* data)
{
    if (i < 0 || i + count >= _size){throw std::out_of_range("");}
    glBufferSubData(i, count, data);
}

template<typename T>
T* BufferArray<T>::_alloc(GLsizeiptr n)
{
    return malloc(n * sizeof(T));
}

template<typename T>
void BufferArray<T>::_free(T* ptr)
{
    free(ptr);
}

}