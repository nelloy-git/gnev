#pragma once

#include "gl/Handler.hpp"

namespace gnev::gl {

class EXPORT Shader : public Handler {
public:
    Shader(GLenum type);
    virtual ~Shader();

    void setSource(GLsizei count, const GLchar* const* string, const GLint* length);
    void compile();

    GLenum getType() const;
    bool hasDeleteFlag() const;
    bool isCompileSucceed() const;
    GLsizei getInfoLogLenth() const;
    GLsizei getSourceLenth() const;
    std::string getInfoLog() const;

private:
    static GLuint* createHandle(GLenum type);
    static void deleteHandle(GLuint* handle);
};

} // namespace gnev::gl