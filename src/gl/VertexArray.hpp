#pragma once

#include "gl/Buffer.hpp"
#include "gl/Handler.hpp"

namespace gnev::gl {

class EXPORT VertexArray : public Handler {
public:
    VertexArray();
    virtual ~VertexArray();

    void bind() const;
    void setElementBuffer(const Buffer& buffer);
    void setVertexBuffer(GLuint binding_index,
                         const Buffer& buffer,
                         GLintptr offset,
                         GLsizei stride);
    void setAttributeBinding(GLuint attrib_index, GLuint binding_index);
    void setAttributeFormat(GLuint attrib_index,
                            GLint size,
                            GLenum type,
                            GLboolean normalized,
                            GLuint relative_offset);
    void setBindingDivisor(GLuint binding_index, GLuint divisor);
    void enableAttribute(GLuint index);
    void disableAttribute(GLuint index);

private:
    static GLuint* createHandle();
    static void deleteHandle(GLuint* handle);
};

} // namespace gnev::gl