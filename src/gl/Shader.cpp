#include "gl/Shader.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/Enum.hpp"
#include "gl/fmt/HandlerLog.hpp"
#include "util/Log.hpp"

using namespace gnev::gl;

Shader::Shader(GLenum type)
    : Handler(createHandle(type), &deleteHandle) {
    Log()->L2();
}

Shader::~Shader() { Log()->L2(); }

void Shader::setSource(GLsizei count, const GLchar* const* string, const GLint* length) {
    Ctx::Get().glShaderSource(handle(), count, string, length);
    Log()->L2(count,
              std::vector<std::string>(string, string + count),
              std::vector(length, length + count));
}

void Shader::compile() {
    Ctx::Get().glCompileShader(handle());
    Log()->L2();
}

GLenum Shader::getType() const {
    GLenum type;
    Ctx::Get().glGetShaderiv(handle(), GL_SHADER_TYPE, reinterpret_cast<GLint*>(&type));
    Log()->L2(fmt::Enum{type});
    return type;
}

bool Shader::hasDeleteFlag() const {
    GLint has_delete_flag;
    Ctx::Get().glGetShaderiv(handle(), GL_DELETE_STATUS, &has_delete_flag);
    Log()->L2(has_delete_flag == GL_TRUE);
    return has_delete_flag == GL_TRUE;
}

bool Shader::isCompileSucceed() const {
    GLint is_compiled;
    Ctx::Get().glGetShaderiv(handle(), GL_COMPILE_STATUS, &is_compiled);
    Log()->L2(is_compiled == GL_TRUE);
    return is_compiled == GL_TRUE;
}

GLsizei Shader::getInfoLogLenth() const {
    GLsizei len;
    Ctx::Get().glGetShaderiv(handle(), GL_INFO_LOG_LENGTH, &len);
    Log()->L2(len);
    return len;
}

GLsizei Shader::getSourceLenth() const {
    GLsizei len;
    Ctx::Get().glGetShaderiv(handle(), GL_SHADER_SOURCE_LENGTH, &len);
    Log()->L2(len);
    return len;
}

std::string Shader::getInfoLog() const {
    std::string info_log;
    GLsizei len;
    Ctx::Get().glGetShaderiv(handle(), GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        info_log.resize(len);
        Ctx::Get().glGetShaderInfoLog(handle(), len, &len, info_log.data());
    }

    Log()->L2(info_log);
    return info_log;
}

GLuint* Shader::createHandle(GLenum type) {
    return new GLuint(Ctx::Get().glCreateShader(type));
}

void Shader::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteShader(*handle);
    delete handle;
}
