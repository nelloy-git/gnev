#pragma once

#include "gl/Handler.hpp"
#include "gl/Buffer.hpp"

namespace gnev::gl {

class EXPORT VertexArray : public Handler {
public:
    VertexArray();
    virtual ~VertexArray();

    void glBindVertexArray() const;
    void setElementBuffer(const Buffer& buffer);
    void setVertexBuffer(GLuint bindingindex,
                         const Buffer& buffer,
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
    static GLuint* createHandle();
    static void deleteHandle(GLuint* handle);
};

} // namespace gnev::gl