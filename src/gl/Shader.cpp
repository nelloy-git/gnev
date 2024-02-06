#include "gl/Shader.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/Enum.hpp"
#include "gl/logger/HandlerLogger.hpp"

namespace gnev::gl {

Shader::Shader(ShaderType type)
    : Handler(createHandle(type), &deleteHandle) {
    Log()->Func(type);
}

Shader::~Shader() { Log()->Func(); }

void Shader::setSource(GLsizei count, const GLchar* const* string, const GLint* length) {
    Log()->Func(count,
                std::vector<std::string>(string, string + count),
                std::vector(length, length + count));
    Ctx::Get().glShaderSource(handle(), count, string, length);
}

void Shader::compile() {
    Log()->Func();
    Ctx::Get().glCompileShader(handle());
}

ShaderType Shader::getType() const {
    Log()->Func();
    ShaderType type;
    Ctx::Get().glGetShaderiv(handle(), GL_SHADER_TYPE, reinterpret_cast<GLint*>(&type));
    return type;
}

bool Shader::hasDeleteFlag() const {
    Log()->Func();
    GLint has_delete_flag;
    Ctx::Get().glGetShaderiv(handle(), GL_DELETE_STATUS, &has_delete_flag);
    return has_delete_flag == GL_TRUE;
}

bool Shader::isCompileSucceed() const {
    Log()->Func();
    GLint is_compiled;
    Ctx::Get().glGetShaderiv(handle(), GL_COMPILE_STATUS, &is_compiled);
    return is_compiled == GL_TRUE;
}

GLsizei Shader::getInfoLogLenth() const {
    Log()->Func();
    GLsizei len;
    Ctx::Get().glGetShaderiv(handle(), GL_INFO_LOG_LENGTH, &len);
    return len;
}

GLsizei Shader::getSourceLenth() const {
    Log()->Func();
    GLsizei len;
    Ctx::Get().glGetShaderiv(handle(), GL_SHADER_SOURCE_LENGTH, &len);
    return len;
}

std::string Shader::getInfoLog() const {
    Log()->Func();
    std::string info_log;
    GLsizei len;
    Ctx::Get().glGetShaderiv(handle(), GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        info_log.resize(len);
        Ctx::Get().glGetShaderInfoLog(handle(), len, &len, info_log.data());
    }
    return info_log;
}

GLuint* Shader::createHandle(ShaderType type) {
    return new GLuint(Ctx::Get().glCreateShader(static_cast<GLenum>(type)));
}

void Shader::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteShader(*handle);
    delete handle;
}

} // namespace gnev::gl