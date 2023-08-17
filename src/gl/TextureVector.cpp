#include "gl/TextureVector.hpp"

#include <iostream>
#include <stdexcept>

using namespace gnev::gl;

TextureVector::TextureVector(const std::shared_ptr<GladGLContext>& ctx, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
    : Texture(ctx, GL_TEXTURE_2D_ARRAY),
      _img_levels(levels),
      _img_internalformat(internalformat),
      _size(0),
      _cap(base_cap)
{
    glTextureStorage3D(levels, internalformat, width, height, _cap);
}

TextureVector::~TextureVector()
{
}

void TextureVector::set(GLsizeiptr i, const Image& img)
{
    if (i < 0 || i >= _size){throw std::out_of_range("");}
    glTextureSubImage3D(img.level,
                        img.x, img.y, i,
                        std::min(_get_width(img.level), img.width), std::min(_get_height(img.level), img.height), 1,
                        img.format, img.type, img.data.get());
}

GLsizeiptr TextureVector::size() const
{
    return _size;
}

void TextureVector::reserve(GLsizeiptr capacity)
{
    if (capacity < 0 || capacity > max_size()){throw std::out_of_range("");}

    if (_cap >= capacity){
        return;
    }

    _cap = capacity;
    if (_size == 0){
        glTextureStorage3D(_img_levels, _img_internalformat, _get_width(0), _get_height(0), _cap);
        return;
    }
    
    Texture tmp(ctx(), GL_TEXTURE_2D_ARRAY);
    tmp.glTextureStorage3D(_img_levels, _img_internalformat, _get_width(0), _get_height(0), _size);
    for (GLsizei level = 0; level < _img_levels; ++level){
        glCopyImageSubData(GL_TEXTURE_2D_ARRAY, level,
                           0, 0, 0,
                           tmp.handle(), GL_TEXTURE_2D_ARRAY, level,
                           0, 0, 0,
                           _get_width(level), _get_height(level), _size);
    }
    glTextureStorage3D(_img_levels, _img_internalformat, _get_width(0), _get_height(0), _cap);
    for (GLsizei level = 0; level < _img_levels; ++level){
        tmp.glCopyImageSubData(GL_TEXTURE_2D_ARRAY, level,
                               0, 0, 0,
                               handle(), GL_TEXTURE_2D_ARRAY, level,
                               0, 0, 0,
                               _get_width(level), _get_height(level), _size);
    }
}

GLsizeiptr TextureVector::capacity() const
{
    return _cap;
}

void TextureVector::shrink_to_fit()
{
    _cap = _size;
    if (_size == 0){
        glTextureStorage3D(_img_levels, _img_internalformat, _get_width(0), _get_height(0), _cap);
        return;
    }
    
    Texture tmp(ctx(), GL_TEXTURE_2D_ARRAY);
    tmp.glTextureStorage3D(_img_levels, _img_internalformat, _get_width(0), _get_height(0), _size);
    for (GLsizei level = 0; level < _img_levels; ++level){
        glCopyImageSubData(GL_TEXTURE_2D_ARRAY, level,
                           0, 0, 0,
                           tmp.handle(), GL_TEXTURE_2D_ARRAY, level,
                           0, 0, 0,
                           _get_width(level), _get_height(level), _size);
    }
    glTextureStorage3D(_img_levels, _img_internalformat, _get_width(0), _get_height(0), _cap);
    for (GLsizei level = 0; level < _img_levels; ++level){
        tmp.glCopyImageSubData(GL_TEXTURE_2D_ARRAY, level,
                               0, 0, 0,
                               handle(), GL_TEXTURE_2D_ARRAY, level,
                               0, 0, 0,
                               _get_width(level), _get_height(level), _size);
    }
}

GLsizeiptr TextureVector::max_size() const
{
    GLint size;
    ctx()->GetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &size);
    return size;
}

void TextureVector::insert(GLsizeiptr i, const Image& value)
{
    insert_range(i, &value, 1);
}

void TextureVector::insert_range(GLsizeiptr pos, const Image* value, GLuint count) 
{
    if (pos < 0 || pos > _size || _size + count > max_size()){throw std::out_of_range("");}

    auto level = value[0].level;
    for (GLuint i = 0; i < count; ++i){
        if (value->level != level){throw std::out_of_range("");}
    }

    while (_size + count > _cap){
        reserve(_cap * _cap_mult + _cap_add);
    }

    // push_back
    _size += count;
    if (pos == _size){
        for (GLsizei i = 0; i < count; ++i){
            set(_size + i, value[i]);
        }
    }

    // Move existing data
    for (GLsizei level = 0; level < _img_levels; ++level){
        glCopyImageSubData(GL_TEXTURE_2D_ARRAY, level,
                           0, 0, pos,
                           handle(), GL_TEXTURE_2D_ARRAY, level,
                           0, 0, pos + count,
                           _get_width(level), _get_height(level), _size - count - pos);
    }

    // Apply insertion
    for (GLsizei i = 0; i < count; ++i){
        set(_size + i, value[i]);
    }
}

void TextureVector::push_back(const Image& value)
{
    insert_range(_size, &value, 1);
}

void TextureVector::push_back_range(const Image* value, GLuint count)
{
    insert_range(_size, value, count);
}

// void TextureVector::set(GLsizei i, GLint level, GLenum format, GLenum type, const void* img)
// {
//     if (i >= _size){
//         throw std::out_of_range("");
//     }
//     glTextureSubImage3D(level, 0, 0, i, get_level_parameteri(level, GL_TEXTURE_WIDTH), get_level_parameteri(level, GL_TEXTURE_HEIGHT), 1, format, type, img);
// }

// void TextureVector::push_back(GLint level, GLenum format, GLenum type, const void* img)
// {
//     while (_size >= _cap){
//         reserve(_cap * CAP_MULT + CAP_ADD);
//     }

//     ++_size;
//     set(_size-1, level, format, type, img);
// }

GLint TextureVector::_get_width(GLint level) const
{
    GLint param;
    glGetTextureLevelParameteriv(level, GL_TEXTURE_WIDTH, &param);
    return param;
}

GLint TextureVector::_get_height(GLint level) const
{
    GLint param;
    glGetTextureLevelParameteriv(level, GL_TEXTURE_HEIGHT, &param);
    return param;
}

void* TextureVector::_alloc(size_t size)
{
    return malloc(size);
}

void TextureVector::_free(void* ptr)
{
    free(ptr);
}