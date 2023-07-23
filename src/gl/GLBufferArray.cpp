#include "gl/GLBufferArray.hpp"

using namespace gnev;

GLBufferArray::GLBufferArray(const std::shared_ptr<GladGLContext> &ctx,
                               const void* initial_data, GLsizeiptr initial_size, GLbitfield storage_flags)
    : GLBuffer(ctx),
      _size(initial_size)
{
    glBufferStorage(_size, initial_data, storage_flags);
}

GLBufferArray::~GLBufferArray()
{
}

GLsizeiptr GLBufferArray::size() const
{
    return _size;
}

std::shared_ptr<void> GLBufferArray::get(GLintptr offset, GLsizeiptr size) const
{
    if (offset + size > _size){
        throw std::out_of_range("");
    }

    auto data = std::shared_ptr<void>(malloc(size));
    glGetBufferSubData(offset, size, data.get());
    return data;
}

void GLBufferArray::set(GLintptr offset, GLsizeiptr size, const void* data)
{
    if (offset + size > _size){
        throw std::out_of_range("");
    }
    glBufferSubData(offset, size, data);
}