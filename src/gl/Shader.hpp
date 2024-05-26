#pragma once

#include "gl/Handler.hpp"
#include "enum/ShaderType.hpp"

namespace gnev::gl {

class EXPORT Shader : public Handler {
public:
    Shader(ShaderType type);
    virtual ~Shader();

    void setSource(GLsizei count, const GLchar* const* string, const GLint* length);
    void compile();

    ShaderType getType() const;
    bool hasDeleteFlag() const;
    bool isCompileSucceed() const;
    GLsizei getInfoLogLenth() const;
    GLsizei getSourceLenth() const;
    std::string getInfoLog() const;

private:
    static GLuint* createHandle(ShaderType type);
    static void deleteHandle(GLuint* handle);
};

std::string format_as(const Shader&);

} // namespace gnev::gl