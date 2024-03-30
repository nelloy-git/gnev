#pragma once

#include "gl/Handler.hpp"
#include "gl/enum/BufferMapRangeAccess.hpp"
#include "gl/enum/BufferStorageFlags.hpp"

namespace gnev::gl {

class EXPORT Buffer : public Handler {
public:
    Buffer();
    Buffer(const Buffer& other) = delete;
    Buffer(Buffer&& other) = default;
    virtual ~Buffer();

    void bind(GLenum target) const;
    void bindBase(GLenum target, GLuint index) const;
    void bindRange(GLenum target, GLuint index, GLintptr offset, GLsizeiptr size) const;
    void initData(GLsizeiptr size, const void* data, GLenum usage);
    void initStorage(GLsizeiptr size, const void* data, BufferStorageFlags flags);
    void setSubData(GLintptr offset, GLsizeiptr size, const void* data);
    void getSubData(GLintptr offset, GLsizeiptr size, void* data) const;
    void copyTo(Buffer& writeBuffer,
                GLintptr readOffset,
                GLintptr writeOffset,
                GLsizeiptr size) const;

    GLint getSize() const;
    bool isStorage() const;
    BufferStorageFlags getStorageFlags() const;

    void* map(GLenum access);
    void* mapRange(GLintptr offset, GLsizeiptr length, BufferMapRangeAccess access);
    void flushRange(GLintptr offset, GLsizeiptr length);
    void unmap();

private:
    static GLuint* createHandle();
    static void deleteHandle(GLuint* handle);
};

} // namespace gnev::gl