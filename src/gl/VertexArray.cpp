#include "gl/VertexArray.hpp"

using namespace gnev::gl;

VertexArray::VertexArray()
    : Handler(createHandle(), &deleteHandle) {}

VertexArray::~VertexArray() {}

void VertexArray::glBindVertexArray() const { Ctx::Get().glBindVertexArray(handle()); }

void VertexArray::setElementBuffer(const Buffer& buffer) {
    Ctx::Get().glVertexArrayElementBuffer(handle(), buffer.handle());
}

void VertexArray::setVertexBuffer(GLuint bindingindex,
                                  const Buffer& buffer,
                                  GLintptr offset,
                                  GLsizei stride) {
    Ctx::Get().glVertexArrayVertexBuffer(handle(),
                                         bindingindex,
                                         buffer.handle(),
                                         offset,
                                         stride);
}

void VertexArray::glVertexArrayAttribBinding(GLuint attribindex, GLuint bindingindex) {
    Ctx::Get().glVertexArrayAttribBinding(handle(), attribindex, bindingindex);
}

void VertexArray::glVertexArrayAttribFormat(GLuint attribindex,
                                            GLint size,
                                            GLenum type,
                                            GLboolean normalized,
                                            GLuint relativeoffset) {
    Ctx::Get().glVertexArrayAttribFormat(handle(),
                                         attribindex,
                                         size,
                                         type,
                                         normalized,
                                         relativeoffset);
}

void VertexArray::glVertexArrayBindingDivisor(GLuint bindingindex, GLuint divisor) {
    Ctx::Get().glVertexArrayBindingDivisor(handle(), bindingindex, divisor);
}

void VertexArray::glEnableVertexArrayAttrib(GLuint index) {
    Ctx::Get().glEnableVertexArrayAttrib(handle(), index);
}

void VertexArray::glDisableVertexArrayAttrib(GLuint index) {
    Ctx::Get().glDisableVertexArrayAttrib(handle(), index);
}

GLuint* VertexArray::createHandle() {
    auto handle = new GLuint(0);
    Ctx::Get().glCreateVertexArrays(1, handle);
    return handle;
}

void VertexArray::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteVertexArrays(1, handle);
    delete handle;
}