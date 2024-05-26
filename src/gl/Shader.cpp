#include "gl/Shader.hpp"

namespace gnev::gl {

Shader::Shader(ShaderType type)
    : Handler(createHandle(type), &deleteHandle) {
    GNEV_HANDLER_LOG(L2, type);
}

Shader::~Shader() { GNEV_HANDLER_LOG(L2); }

void Shader::setSource(GLsizei count, const GLchar* const* string, const GLint* length) {
    GNEV_HANDLER_LOG(L2,
                     count,
                     std::vector<std::string>(string, string + count),
                     std::vector(length, length + count));
    Ctx::Get().glShaderSource(handle(), count, string, length);
}

void Shader::compile() {
    GNEV_HANDLER_LOG(L2);
    Ctx::Get().glCompileShader(handle());
}

ShaderType Shader::getType() const {
    GNEV_HANDLER_LOG(L2);
    ShaderType type;
    Ctx::Get().glGetShaderiv(handle(), GL_SHADER_TYPE, reinterpret_cast<GLint*>(&type));
    GNEV_LOG(L2, "\treturn {}", type);
    return type;
}

bool Shader::hasDeleteFlag() const {
    GNEV_HANDLER_LOG(L2);
    GLint has_delete_flag;
    Ctx::Get().glGetShaderiv(handle(), GL_DELETE_STATUS, &has_delete_flag);
    GNEV_LOG(L2, "\treturn {}", (has_delete_flag == GL_TRUE));
    return has_delete_flag == GL_TRUE;
}

bool Shader::isCompileSucceed() const {
    GNEV_HANDLER_LOG(L2);
    GLint is_compiled;
    Ctx::Get().glGetShaderiv(handle(), GL_COMPILE_STATUS, &is_compiled);
    GNEV_LOG(L2, "\treturn {}", (is_compiled == GL_TRUE));
    return is_compiled == GL_TRUE;
}

GLsizei Shader::getInfoLogLenth() const {
    GNEV_HANDLER_LOG(L2);
    GLsizei len;
    Ctx::Get().glGetShaderiv(handle(), GL_INFO_LOG_LENGTH, &len);
    GNEV_LOG(L2, "\treturn {}", len);
    return len;
}

GLsizei Shader::getSourceLenth() const {
    GNEV_HANDLER_LOG(L2);
    GLsizei len;
    Ctx::Get().glGetShaderiv(handle(), GL_SHADER_SOURCE_LENGTH, &len);
    GNEV_LOG(L2, "\treturn {}", len);
    return len;
}

std::string Shader::getInfoLog() const {
    GNEV_HANDLER_LOG(L2);
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

std::string format_as(const Shader& shader) {
    return "Shader#" + fmtquill::to_string(shader.handle());
}

} // namespace gnev::gl