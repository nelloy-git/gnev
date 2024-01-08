#include "gl/Shader.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/Enum.hpp"
#include "util/Log.hpp"

using namespace gnev::gl;

Shader::Shader(GLenum type)
    : Handler(createHandle(type), &deleteHandle) {
    GNEV_TRACE_L2("Shader_{}::ctor()", handle());
}

Shader::~Shader() { GNEV_TRACE_L2("Shader_{}::destr()", handle()); }

void Shader::setSource(GLsizei count, const GLchar* const* string, const GLint* length) {
    Ctx::Get().glShaderSource(handle(), count, string, length);
    GNEV_TRACE_L2("Shader_{}::setSource({}, {}, {})",
                  handle(),
                  count,
                  std::vector<std::string>(string, string + count),
                  std::vector(length, length + count));
}

void Shader::compile() {
    Ctx::Get().glCompileShader(handle());
    GNEV_TRACE_L2("Shader_{}::compile()", handle());
}

GLenum Shader::getType() const {
    GLenum type;
    Ctx::Get().glGetShaderiv(handle(), GL_SHADER_TYPE, reinterpret_cast<GLint*>(&type));
    GNEV_TRACE_L2("Shader_{}::getType() -> {}", handle(), fmt::Enum{type});
    return type;
}

bool Shader::hasDeleteFlag() const {
    GLint has_delete_flag;
    Ctx::Get().glGetShaderiv(handle(), GL_DELETE_STATUS, &has_delete_flag);
    GNEV_TRACE_L2("Shader_{}::hasDeleteFlag() -> {}",
                  handle(),
                  has_delete_flag == GL_TRUE);
    return has_delete_flag == GL_TRUE;
}

bool Shader::isCompileSucceed() const {
    GLint is_compiled;
    Ctx::Get().glGetShaderiv(handle(), GL_COMPILE_STATUS, &is_compiled);
    GNEV_TRACE_L2("Shader_{}::isCompiled() -> {}", handle(), is_compiled == GL_TRUE);
    return is_compiled == GL_TRUE;
}

GLsizei Shader::getInfoLogLenth() const {
    GLsizei len;
    Ctx::Get().glGetShaderiv(handle(), GL_INFO_LOG_LENGTH, &len);
    GNEV_TRACE_L2("Shader_{}::getInfoLogLenth() -> {}", handle(), len);
    return len;
}

GLsizei Shader::getSourceLenth() const {
    GLsizei len;
    Ctx::Get().glGetShaderiv(handle(), GL_SHADER_SOURCE_LENGTH, &len);
    GNEV_TRACE_L2("Shader_{}::getSourceLenth() -> {}", handle(), len);
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

    GNEV_TRACE_L2("Shader_{}::getInfoLog() -> \"{}\"", handle(), info_log);
    return info_log;
}

GLuint* Shader::createHandle(GLenum type) {
    return new GLuint(Ctx::Get().glCreateShader(type));
}

void Shader::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteShader(*handle);
    delete handle;
}
