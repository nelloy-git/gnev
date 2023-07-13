#include "texture/TextureChunk.hpp"

#include <iostream>
#include <stdexcept>

using namespace gnev;

GLint TextureChunk::MAX_CAPACITY(GladGLContext& ctx){
    const thread_local GLint MAX_CAP = [&ctx](){
        GLint val;
        ctx.GetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &val);
        return val;
    }();
    return MAX_CAP;
}

TextureChunk::TextureChunk(const std::shared_ptr<GladGLContext>& ctx) :
    _ctx(ctx){
}

TextureChunk::~TextureChunk(){
}

std::pair<bool, std::string> TextureChunk::init(GLsizei width, GLsizei height){
    _width = width;
    _height = height;

    _texture_array = std::make_unique<GLTexture>(_ctx, GL_TEXTURE_2D_ARRAY);
    _texture_array->glTextureStorage3D(1, GL_RGBA8, width, height, CAPACITY_BASE);

    return {true, ""};
}

bool TextureChunk::expand(GLsizei capacity){
    if (_capacity == MAX_CAPACITY(*_ctx)){
        return false;
    }
    _capacity = std::min(MAX_CAPACITY(*_ctx), capacity);

    auto new_texture_array = std::make_unique<GLTexture>(_ctx, GL_TEXTURE_2D_ARRAY);
    new_texture_array->glTextureStorage3D(1, GL_RGBA8, _width, _height, capacity);
    _texture_array->glCopyImageSubData(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, new_texture_array->handle, GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, _width, _height, _size);
    _texture_array = std::move(new_texture_array);
    return true;
}

GLint TextureChunk::add(const void* data){
    if (_size >= _capacity){
        _capacity *= CAPACITY_MULT;
        if (!expand(_capacity)){
            return -1;
        }
    }
    GLint id = _size;
    ++_size;
    _texture_array->glTextureSubImage3D(0, 0, 0, id, _width, _height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    return id;
}

GLsizei TextureChunk::width() const {
    return _width;
}

GLsizei TextureChunk::height() const {
    return _height;
}

GLsizei TextureChunk::size() const {
    return _size;
}

GLsizei TextureChunk::capacity() const {
    return _capacity;
}

GLint TextureChunk::handle() const {
    return _texture_array->handle;
}