#include "gl/GLBufferVectorCoherent.hpp"

#include <iostream>

using namespace gnev;

GLBufferVectorCoherent::GLBufferVectorCoherent(const std::shared_ptr<GladGLContext> &ctx,
                                               const void* initial_data, size_type initial_size, GLenum usage)
    : GLBuffer(ctx),
      _size(initial_size),
      _cap(std::max(initial_size, CAP_BASE))
{
    glBufferData(_cap, initial_data, usage);
    unsigned char** p_map = new unsigned char*();
    auto gl_map = glMapBufferRange(0, _cap, _map_access);
    *p_map = static_cast<unsigned char*>(gl_map);

    std::cout << "Map: " << gl_map << std::endl;

    _map = std::shared_ptr<unsigned char*>(p_map, [this](unsigned char** p_map){glUnmapBuffer(); delete p_map;}) ;
}

GLBufferVectorCoherent::~GLBufferVectorCoherent()
{
    glUnmapBuffer();
}

void* GLBufferVectorCoherent::at(size_type offset)
{
    if (offset > _size){
        throw std::out_of_range("");
    }
    return *_map + offset;
}

const void* GLBufferVectorCoherent::at(size_type offset) const
{
    if (offset > _size){
        throw std::out_of_range("");
    }
    return *_map + offset;
}

void* GLBufferVectorCoherent::data()
{
    return *_map;
}

const void* GLBufferVectorCoherent::data() const
{
    return *_map;
}

bool GLBufferVectorCoherent::empty() const
{
    return _size == 0;
}

GLBufferVectorCoherent::size_type GLBufferVectorCoherent::size() const
{
    return _size;
}

void GLBufferVectorCoherent::reserve(size_type capacity)
{
    if (capacity <= _cap){
        return;
    }

    _cap = capacity;

    GLint usage;
    glGetBufferParameteriv(GL_BUFFER_USAGE, &usage);

    if (_size == 0){
        glBufferData(capacity, nullptr, usage);
    } else {
        auto tmp = std::make_unique<unsigned char[]>(_size);
        memcpy(tmp.get(), *_map, _size);

        GLint usage;
        glGetBufferParameteriv(GL_BUFFER_USAGE, &usage);

        glUnmapBuffer();
        glBufferData(_cap, tmp.get(), usage);
        auto gl_map = glMapBufferRange(0, _cap, _map_access);
        std::cout << "reserve: " << gl_map << std::endl;
        *_map = static_cast<unsigned char*>(gl_map);
    }
}

GLBufferVectorCoherent::size_type GLBufferVectorCoherent::capacity() const
{
    return _cap;
}

void GLBufferVectorCoherent::shrink_to_fit()
{
    if (_cap <= _size || _cap <= CAP_BASE){
        return;
    }

    _cap = std::max(_size, CAP_BASE);

    auto tmp = std::make_unique<unsigned char[]>(_size);
    memcpy(tmp.get(), *_map, _size);

    GLint usage;
    glGetBufferParameteriv(GL_BUFFER_USAGE, &usage);

    glUnmapBuffer();
    glBufferData(_cap, tmp.get(), usage);
    *_map = static_cast<unsigned char*>(glMapBufferRange(0, _cap, _map_access));
}

void GLBufferVectorCoherent::clear()
{
    _size = 0;
}

void GLBufferVectorCoherent::insert(size_type offset, size_type size, const void* data) 
{
    if (offset > _size){
        throw std::out_of_range("");
    }

    while (_size + size > _cap){
        reserve(_cap * CAP_MULT + CAP_ADD);
    }

    memcpy(*_map + offset + size, *_map + offset, size);
    memcpy(*_map + offset, data, size);
    _size += size;
}

void GLBufferVectorCoherent::erase(size_type offset, size_type size)
{
    if (offset + size > _size){
        throw std::out_of_range("");
    }
    
    memcpy(*_map + offset, *_map + offset + size, _size - offset - size);
    _size -= size;
}

void GLBufferVectorCoherent::push_back(size_type size, const void* data)
{
    while (_size + size > _cap){
        reserve(_cap * CAP_MULT + CAP_ADD);
    }
    memcpy(*_map + _size, data, size);
    _size += size;
}

void GLBufferVectorCoherent::pop_back(size_type size)
{
    _size -= size;
}