#include "gl/GLTextureVector.hpp"

#include <iostream>
#include <stdexcept>

using namespace gnev;

GLTextureVector::GLTextureVector(const std::shared_ptr<GladGLContext>& ctx, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
    : GLTexture(ctx, GL_TEXTURE_2D_ARRAY),
      _levels(levels),
      _internalformat(internalformat),
      _size(0),
      _cap(CAP_BASE)
{
    glTextureStorage3D(levels, internalformat, width, height, CAP_BASE);
}

GLTextureVector::~GLTextureVector()
{
}

GLsizeiptr GLTextureVector::size() const
{
    return _size;
}

GLsizeiptr GLTextureVector::capacity() const
{
    return _cap;
}

GLsizeiptr GLTextureVector::max_size() const
{
    GLint size;
    ctx()->GetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &size);
    return size;
}

void GLTextureVector::set(GLsizei i, GLint level, GLenum format, GLenum type, const void* img)
{
    if (i >= _size){
        throw std::out_of_range("");
    }
    glTextureSubImage3D(level, 0, 0, i, get_level_parameteri(level, GL_TEXTURE_WIDTH), get_level_parameteri(level, GL_TEXTURE_HEIGHT), 1, format, type, img);
}

void GLTextureVector::push_back(GLint level, GLenum format, GLenum type, const void* img)
{
    while (_size >= _cap){
        reserve(_cap * CAP_MULT + CAP_ADD);
    }

    ++_size;
    set(_size-1, level, format, type, img);
}

void GLTextureVector::reserve(GLsizei capacity)
{
    if (_cap >= capacity){
        return;
    }

    auto max_cap = max_size();
    if (capacity > max_cap){
        capacity = max_cap;
    }

    if (_size == max_cap){
        throw std::out_of_range("_size == GL_MAX_ARRAY_TEXTURE_LAYERS");
    }
    
    auto width = get_level_parameteri(0, GL_TEXTURE_WIDTH);
    auto height = get_level_parameteri(0, GL_TEXTURE_HEIGHT);
    if (_size > 0){
        GLTexture tmp(ctx(), GL_TEXTURE_2D_ARRAY);
        tmp.glTextureStorage3D(_levels, _internalformat, width, height, _size);
        for (int i = 0; i < _size; ++i){
            glCopyImageSubData(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, tmp.handle(), GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, i);
        }
        glTextureStorage3D(_levels, _internalformat, width, height, capacity);
        for (int i = 0; i < _size; ++i){
            tmp.glCopyImageSubData(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, handle(), GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, i);
        }
    } else {
        glTextureStorage3D(_levels, _internalformat, width, height, capacity);
    }
    _cap = capacity;
}

GLint GLTextureVector::get_level_parameteri(GLint level, GLenum pname) const
{
    GLint param;
    glGetTextureLevelParameteriv(level, pname, &param);
    return param;
}