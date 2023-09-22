#include "gl/VertexArray.hpp"

using namespace gnev::gl;

VertexArray::VertexArray(const std::shared_ptr<GladGLContext>& ctx)
    : Handler(ctx, create_handle(ctx), &handle_deleter) {}

VertexArray::~VertexArray() {}

void VertexArray::glBindVertexArray() const { ctx()->BindVertexArray(handle()); }

void VertexArray::glVertexArrayElementBuffer(GLuint buffer) {
    ctx()->VertexArrayElementBuffer(handle(), buffer);
}

void VertexArray::glVertexArrayVertexBuffer(GLuint bindingindex,
                                            GLuint buffer,
                                            GLintptr offset,
                                            GLsizei stride) {
    ctx()->VertexArrayVertexBuffer(handle(), bindingindex, buffer, offset, stride);
}

void VertexArray::glVertexArrayAttribBinding(GLuint attribindex, GLuint bindingindex) {
    ctx()->VertexArrayAttribBinding(handle(), attribindex, bindingindex);
}

void VertexArray::glVertexArrayAttribFormat(
    GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) {
    ctx()->VertexArrayAttribFormat(handle(), attribindex, size, type, normalized, relativeoffset);
}

void VertexArray::glVertexAttribPointer(GLuint index,
                                        GLint size,
                                        GLenum type,
                                        GLboolean normalized,
                                        GLsizei stride,
                                        const void* pointer) {
    ctx()->BindVertexArray(handle());
    ctx()->VertexAttribPointer(index, size, type, normalized, stride, pointer);
    ctx()->BindVertexArray(0);
}

void VertexArray::glVertexArrayBindingDivisor(GLuint bindingindex, GLuint divisor) {
    ctx()->VertexArrayBindingDivisor(handle(), bindingindex, divisor);
}

void VertexArray::glEnableVertexArrayAttrib(GLuint index) {
    ctx()->EnableVertexArrayAttrib(handle(), index);
}

void VertexArray::glDisableVertexArrayAttrib(GLuint index) {
    ctx()->DisableVertexArrayAttrib(handle(), index);
}

GLuint* VertexArray::create_handle(const std::shared_ptr<GladGLContext>& ctx) {
    GLuint* handle = new GLuint(0);
    if (ctx->VERSION_4_5) {
        ctx->CreateVertexArrays(1, handle);
    } else {
        ctx->GenVertexArrays(1, handle);
    }
    return handle;
}

void VertexArray::handle_deleter(GLuint* handle, GladGLContext& ctx) {
    ctx.DeleteVertexArrays(1, handle);
    delete handle;
}