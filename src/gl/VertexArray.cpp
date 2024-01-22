#include "gl/VertexArray.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/Enum.hpp"
#include "gl/fmt/HandlerLog.hpp"

using namespace gnev::gl;

VertexArray::VertexArray()
    : Handler(createHandle(), &deleteHandle) {
    L2()->log();
}

VertexArray::~VertexArray() { L2()->log(); }

void VertexArray::bind() const {
    L2()->log();
    Ctx::Get().glBindVertexArray(handle());
}

void VertexArray::setElementBuffer(const Buffer& buffer) {
    L2()->log(buffer.handle());
    Ctx::Get().glVertexArrayElementBuffer(handle(), buffer.handle());
}

void VertexArray::setVertexBuffer(GLuint binding_index,
                                  const Buffer& buffer,
                                  GLintptr offset,
                                  GLsizei stride) {
    L2()->log(binding_index, buffer.handle(), offset, stride);
    Ctx::Get().glVertexArrayVertexBuffer(handle(),
                                         binding_index,
                                         buffer.handle(),
                                         offset,
                                         stride);
}

void VertexArray::setAttributeBinding(GLuint attrib_index, GLuint binding_index) {
    L2()->log(attrib_index, binding_index);
    Ctx::Get().glVertexArrayAttribBinding(handle(), attrib_index, binding_index);
}

void VertexArray::setAttributeFormat(GLuint attrib_index,
                                     GLint size,
                                     GLenum type,
                                     GLboolean normalized,
                                     GLuint relative_offset) {
    L2()->log(attrib_index, size, fmt::Enum{type}, normalized, relative_offset);
    Ctx::Get().glVertexArrayAttribFormat(handle(),
                                         attrib_index,
                                         size,
                                         type,
                                         normalized,
                                         relative_offset);
}

void VertexArray::setBindingDivisor(GLuint binding_index, GLuint divisor) {
    L2()->log(binding_index, divisor);
    Ctx::Get().glVertexArrayBindingDivisor(handle(), binding_index, divisor);
}

void VertexArray::enableAttribute(GLuint index) {
    L2()->log(index);
    Ctx::Get().glEnableVertexArrayAttrib(handle(), index);
}

void VertexArray::disableAttribute(GLuint index) {
    L2()->log(index);
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