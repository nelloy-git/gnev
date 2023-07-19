#pragma once

#include "gl/GLHandler.hpp"

namespace gnev {

class EXPORT GLVertexArray : public GLHandler {
public:
    GLVertexArray(const std::shared_ptr<GladGLContext> &ctx);
    virtual ~GLVertexArray();

    void glBindVertexArray() const;
    void glVertexArrayElementBuffer(GLuint buffer);
    void glVertexArrayVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
    void glVertexArrayAttribBinding(GLuint attribindex, GLuint bindingindex);
    void glVertexArrayAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
    void glVertexArrayBindingDivisor(GLuint bindingindex, GLuint divisor);
    void glEnableVertexArrayAttrib(GLuint index);
    void glDisableVertexArrayAttrib(GLuint index);

private:
    static GLuint* create_handle(const std::shared_ptr<GladGLContext> &ctx);
    static void handle_deleter(GLuint* handle, GladGLContext& ctx);
};

}