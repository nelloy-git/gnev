#pragma once

#include <stdexcept>

#include "gl/BufferArray.hpp"

namespace gnev::gl {

template<typename T>
class EXPORT BufferArrayCoherent : public BufferArray<T> {
public:
    BufferArrayCoherent(const GladCtx& ctx, GLsizeiptr size, const T* data);
    BufferArrayCoherent(const GladCtx& ctx, const std::initializer_list<T>& data);
    virtual ~BufferArrayCoherent();

    T* data();
    const T* data() const;

    T& operator[](GLsizeiptr i);
    const T& operator[](GLsizeiptr i) const;

    T& get(GLsizeiptr i);
    const T& get(GLsizeiptr i) const;
    void set(GLsizeiptr i, const T& value);

    void set_range(GLsizeiptr i, GLsizeiptr count, const T* data);

private:
    static constexpr GLbitfield _storage_flags = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
    using BufferArray<T>::get_range;

    T* _map;
};

template<typename T>
BufferArrayCoherent<T>::BufferArrayCoherent(const GladCtx& ctx, GLsizeiptr size, const T* data)
    : BufferArray<T>(ctx, size, data, _storage_flags),
      _map(BufferArray<T>::glMapBufferRange(0, size * sizeof(T), _storage_flags))
{

}

template<typename T>
BufferArrayCoherent<T>::BufferArrayCoherent(const GladCtx& ctx, const std::initializer_list<T>& data)
    : BufferArray<T>(ctx, data.size(), data.begin(), _storage_flags),
      _map(static_cast<T*>(BufferArray<T>::glMapBufferRange(0, data.size() * sizeof(T), _storage_flags)))
{
}

template<typename T>
BufferArrayCoherent<T>::~BufferArrayCoherent()
{
}

template<typename T>
inline T* BufferArrayCoherent<T>::data()
{
    return _map;
}

template<typename T>
inline const T* BufferArrayCoherent<T>::data() const
{
    return _map;
}

template<typename T>
inline T& BufferArrayCoherent<T>::operator[](GLsizeiptr i)
{
    return get(i);
}

template<typename T>
inline const T& BufferArrayCoherent<T>::operator[](GLsizeiptr i) const
{
    return get(i);
}

template<typename T>
T& BufferArrayCoherent<T>::get(GLsizeiptr i)
{
    if (i < 0 || i >= BufferArray<T>::size()){throw std::out_of_range("");}
    return _map[i];
}

template<typename T>
const T& BufferArrayCoherent<T>::get(GLsizeiptr i) const
{
    if (i < 0 || i >= BufferArray<T>::size()){throw std::out_of_range("");}
    return _map[i];
}

template<typename T>
void BufferArrayCoherent<T>::set(GLsizeiptr i, const T& value)
{
    if (i < 0 || i >= BufferArray<T>::size()){throw std::out_of_range("");}
    _map[i] = value;
}

template<typename T>
void BufferArrayCoherent<T>::set_range(GLsizeiptr i, GLsizeiptr count, const T* data)
{
    if (i < 0 || i + count >= BufferArray<T>::size()){throw std::out_of_range("");}
    glBufferSubData(i, count, data);
}

}