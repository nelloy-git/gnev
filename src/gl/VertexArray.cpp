#include "gl/VertexArray.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/Enum.hpp"
#include "gl/fmt/HandlerLog.hpp"

using namespace gnev::gl;

VertexArray::VertexArray()
    : Handler(createHandle(), &deleteHandle) {
    Log()->L2();
}

VertexArray::~VertexArray() { Log()->L2(); }

void VertexArray::bind() const {
    Log()->L2();
    Ctx::Get().glBindVertexArray(handle());
}

void VertexArray::setElementBuffer(const Buffer& buffer) {
    Log()->L2(buffer.handle());
    Ctx::Get().glVertexArrayElementBuffer(handle(), buffer.handle());
}

void VertexArray::setVertexBuffer(GLuint binding_index,
                                  const Buffer& buffer,
                                  GLintptr offset,
                                  GLsizei stride) {
    Log()->L2(binding_index, buffer.handle(), offset, stride);
    Ctx::Get().glVertexArrayVertexBuffer(handle(),
                                         binding_index,
                                         buffer.handle(),
                                         offset,
                                         stride);
}

void VertexArray::setAttributeBinding(GLuint attrib_index, GLuint binding_index) {
    Log()->L2(attrib_index, binding_index);
    Ctx::Get().glVertexArrayAttribBinding(handle(), attrib_index, binding_index);
}

void VertexArray::setAttributeFormat(GLuint attrib_index,
                                     GLint size,
                                     GLenum type,
                                     GLboolean normalized,
                                     GLuint relative_offset) {
    Log()->L2(attrib_index, size, fmt::Enum{type}, normalized, relative_offset);
    Ctx::Get().glVertexArrayAttribFormat(handle(),
                                         attrib_index,
                                         size,
                                         type,
                                         normalized,
                                         relative_offset);
}

void VertexArray::setBindingDivisor(GLuint binding_index, GLuint divisor) {
    Log()->L2(binding_index, divisor);
    Ctx::Get().glVertexArrayBindingDivisor(handle(), binding_index, divisor);
}

void VertexArray::enableAttribute(GLuint index) {
    Log()->L2(index);
    Ctx::Get().glEnableVertexArrayAttrib(handle(), index);
}

void VertexArray::disableAttribute(GLuint index) {
    Log()->L2(index);
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