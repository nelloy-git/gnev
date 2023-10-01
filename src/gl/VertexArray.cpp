#include "gl/VertexArray.hpp"

using namespace gnev::gl;

VertexArray::VertexArray(const Ctx& ctx)
    : Handler(ctx, createHandle(ctx), &freeHandle) {}

VertexArray::~VertexArray() {}

void VertexArray::glBindVertexArray() const { ctx().glBindVertexArray(handle()); }

void VertexArray::glVertexArrayElementBuffer(GLuint buffer) {
    ctx().glVertexArrayElementBuffer(handle(), buffer);
}

void VertexArray::glVertexArrayVertexBuffer(GLuint bindingindex,
                                            GLuint buffer,
                                            GLintptr offset,
                                            GLsizei stride) {
    ctx().glVertexArrayVertexBuffer(handle(), bindingindex, buffer, offset, stride);
}

void VertexArray::glVertexArrayAttribBinding(GLuint attribindex, GLuint bindingindex) {
    ctx().glVertexArrayAttribBinding(handle(), attribindex, bindingindex);
}

void VertexArray::glVertexArrayAttribFormat(GLuint attribindex,
                                            GLint size,
                                            GLenum type,
                                            GLboolean normalized,
                                            GLuint relativeoffset) {
    ctx().glVertexArrayAttribFormat(handle(),
                                    attribindex,
                                    size,
                                    type,
                                    normalized,
                                    relativeoffset);
}

void VertexArray::glVertexArrayBindingDivisor(GLuint bindingindex, GLuint divisor) {
    ctx().glVertexArrayBindingDivisor(handle(), bindingindex, divisor);
}

void VertexArray::glEnableVertexArrayAttrib(GLuint index) {
    ctx().glEnableVertexArrayAttrib(handle(), index);
}

void VertexArray::glDisableVertexArrayAttrib(GLuint index) {
    ctx().glDisableVertexArrayAttrib(handle(), index);
}

GLuint VertexArray::createHandle(const Ctx& ctx) {
    GLuint handle;
    ctx.glCreateVertexArrays(1, &handle);
    return handle;
}

void VertexArray::freeHandle(const Ctx& ctx, GLuint handle) {
    ctx.glDeleteVertexArrays(1, &handle);
}