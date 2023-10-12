#pragma once

#include "gl/Handler.hpp"

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
    void initStorage(GLsizeiptr size, const void* data, GLbitfield flags);
    void setSubData(GLintptr offset, GLsizeiptr size, const void* data);
    void getSubData(GLintptr offset, GLsizeiptr size, void* data) const;
    void copyTo(Buffer& writeBuffer,
                GLintptr readOffset,
                GLintptr writeOffset,
                GLsizeiptr size) const;
    void getParameteriv(GLenum pname, GLint* params) const;
    void* map(GLenum access);
    void* mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access);
    void flushRange(GLintptr offset, GLsizeiptr length);
    void unmap();

private:
    static GLuint* createHandle();
    static void deleteHandle(GLuint* handle);
};

} // namespace gnev::gl