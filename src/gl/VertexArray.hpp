#pragma once

#include "gl/Handler.hpp"

namespace gnev::gl {

class EXPORT VertexArray : public Handler {
public:
    VertexArray(const Ctx& ctx);
    virtual ~VertexArray();

    void glBindVertexArray() const;
    void glVertexArrayElementBuffer(GLuint buffer);
    void glVertexArrayVertexBuffer(GLuint bindingindex,
                                   GLuint buffer,
                                   GLintptr offset,
                                   GLsizei stride);
    void glVertexArrayAttribBinding(GLuint attribindex, GLuint bindingindex);
    void glVertexArrayAttribFormat(GLuint attribindex,
                                   GLint size,
                                   GLenum type,
                                   GLboolean normalized,
                                   GLuint relativeoffset);
    void glVertexArrayBindingDivisor(GLuint bindingindex, GLuint divisor);
    void glEnableVertexArrayAttrib(GLuint index);
    void glDisableVertexArrayAttrib(GLuint index);

private:
    static GLuint createHandle(const Ctx& ctx);
    static void freeHandle(const Ctx& ctx, GLuint handle);
};

} // namespace gnev::gl