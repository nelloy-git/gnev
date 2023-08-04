#include "gl/GLBufferVector.hpp"

using namespace gnev;

GLBufferVector::GLBufferVector(const std::shared_ptr<GladGLContext> &ctx,
                               const void* initial_data, GLsizeiptr initial_size, GLenum usage)
    : GLBuffer(ctx),
      _size(initial_size),
      _cap(std::max(initial_size, CAP_BASE))
{
    glBufferData(_cap, initial_data, usage);
}

GLBufferVector::~GLBufferVector()
{

}

GLsizeiptr GLBufferVector::size() const
{
    return _size;
}

GLsizeiptr GLBufferVector::capacity() const
{
    return _cap;
}

std::shared_ptr<void> GLBufferVector::get(GLintptr offset, GLsizeiptr size) const
{
    if (offset + size > _size){
        throw std::out_of_range("");
    }

    auto data = std::shared_ptr<void>(malloc(size));
    glGetBufferSubData(offset, size, data.get());
    return data;
}

void GLBufferVector::set(GLintptr offset, GLsizeiptr size, const void* data)
{
    if (offset + size > _size){
        throw std::out_of_range("");
    }
    glBufferSubData(offset, size, data);
}

void GLBufferVector::push_back(GLsizeiptr size, const void* data)
{
    while (_size + size > _cap){
        reserve(_cap * CAP_MULT + CAP_ADD);
    }
    auto offset = _size;
    _size += size;
    set(offset, size, data);
}

void GLBufferVector::pop_back(GLsizeiptr size)
{
    _size -= size;
}

void GLBufferVector::insert(GLintptr offset, GLsizeiptr size, const void* data) 
{
    while (_size + size > _cap){
        reserve(_cap * CAP_MULT + CAP_ADD);
    }
    glCopyBufferSubData(handle(), offset, offset + size, size);
    _size += size;
    set(offset, size, data);
}

void GLBufferVector::remove(GLintptr offset, GLsizeiptr size)
{
    if (offset + size > _size){
        throw std::out_of_range("");
    }

    glCopyBufferSubData(handle(), offset + size, offset, size);
    _size -= size;
}

void GLBufferVector::reserve(GLsizeiptr capacity)
{
    if (_cap >= capacity){
        return;
    }

    GLint usage;
    glGetBufferParameteriv(GL_BUFFER_USAGE, &usage);

    if (_size > 0){
        GLBuffer tmp_buffer(ctx());
        tmp_buffer.glBufferStorage(_size, nullptr, 0);
        glCopyBufferSubData(tmp_buffer.handle(), 0, 0, _size);

        glBufferData(capacity, nullptr, usage);
        tmp_buffer.glCopyBufferSubData(handle(), 0, 0, _size);
    } else {
        glBufferData(capacity, nullptr, usage);
    }
    _cap = capacity;
}

void GLBufferVector::shrink_to_fit()
{
    GLBuffer tmp_buffer(ctx());
    tmp_buffer.glBufferStorage(_size, nullptr, 0);
    glCopyBufferSubData(tmp_buffer.handle(), 0, 0, _size);
    
    GLint usage;
    glGetBufferParameteriv(GL_BUFFER_USAGE, &usage);
    glBufferData(_size, nullptr, usage);
    tmp_buffer.glCopyBufferSubData(handle(), 0, 0, _size);
    _cap = _size;
}