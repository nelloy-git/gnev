#include "gl/Shader.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/Enum.hpp"
#include "gl/fmt/HandlerTraceL2.hpp"
#include "util/Log.hpp"

using namespace gnev::gl;

Shader::Shader(GLenum type)
    : Handler(createHandle(type), &deleteHandle) {
    L2()->log();
}

Shader::~Shader() { L2()->log(); }

void Shader::setSource(GLsizei count, const GLchar* const* string, const GLint* length) {
    Ctx::Get().glShaderSource(handle(), count, string, length);
    L2()->log(count,
              std::vector<std::string>(string, string + count),
              std::vector(length, length + count));
}

void Shader::compile() {
    Ctx::Get().glCompileShader(handle());
    L2()->log();
}

GLenum Shader::getType() const {
    GLenum type;
    Ctx::Get().glGetShaderiv(handle(), GL_SHADER_TYPE, reinterpret_cast<GLint*>(&type));
    L2()->log(fmt::Enum{type});
    return type;
}

bool Shader::hasDeleteFlag() const {
    GLint has_delete_flag;
    Ctx::Get().glGetShaderiv(handle(), GL_DELETE_STATUS, &has_delete_flag);
    L2()->log(has_delete_flag == GL_TRUE);
    return has_delete_flag == GL_TRUE;
}

bool Shader::isCompileSucceed() const {
    GLint is_compiled;
    Ctx::Get().glGetShaderiv(handle(), GL_COMPILE_STATUS, &is_compiled);
    L2()->log(is_compiled == GL_TRUE);
    return is_compiled == GL_TRUE;
}

GLsizei Shader::getInfoLogLenth() const {
    GLsizei len;
    Ctx::Get().glGetShaderiv(handle(), GL_INFO_LOG_LENGTH, &len);
    L2()->log(len);
    return len;
}

GLsizei Shader::getSourceLenth() const {
    GLsizei len;
    Ctx::Get().glGetShaderiv(handle(), GL_SHADER_SOURCE_LENGTH, &len);
    L2()->log(len);
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

    L2()->log(info_log);
    return info_log;
}

GLuint* Shader::createHandle(GLenum type) {
    return new GLuint(Ctx::Get().glCreateShader(type));
}

void Shader::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteShader(*handle);
    delete handle;
}
