#include "ProgramBuilder.hpp"

#include <iostream>
#include <vector>

using namespace gnev;

ProgramBuilder::ProgramBuilder() {}

ProgramBuilder::~ProgramBuilder() {}

const std::string& ProgramBuilder::reason() const { return _reason; }

const std::string& ProgramBuilder::help() const { return _help; }

Ptr<gl::Program>
ProgramBuilder::build(const std::unordered_map<GLenum, std::string>& sources) {
    _reason = "";
    _help = "";

    Ptr<gl::Program> program = std::make_shared<gl::Program>();
    std::vector<gl::Shader> shaders;
    for (auto& shader_info : sources) {
        gl::Shader shader(shader_info.first);
        auto shader_status = compile_shader(shader, shader_info.second);
        if (!shader_status) {
            return nullptr;
        }
        program->attach(shader);
    }

    auto link_status = link_program(*program);
    if (!link_status) {
        return nullptr;
    }

    auto validate_status = validate_program(*program);
    if (!validate_status) {
        return nullptr;
    }

    return program;
}

bool ProgramBuilder::compile_shader(gl::Shader& shader, const std::string& src) {
    auto c_str = src.c_str();
    auto str_len = static_cast<GLint>(src.size());
    shader.glShaderSource(1, &c_str, &str_len);

    shader.glCompileShader();
    GLint status;
    shader.glGetShaderiv(GL_COMPILE_STATUS, &status);

    GLint len;
    shader.glGetShaderiv(GL_INFO_LOG_LENGTH, &len);

    std::string dst;
    dst.resize(len);
    shader.glGetShaderInfoLog(len, &len, dst.data());

    if (status) {
        _help += dst;
    } else {
        _reason = dst;
    }

    return status;
}

bool ProgramBuilder::link_program(gl::Program& program) {
    program.link();

    GLint status = program.isLinkSucceed();
    std::string info_log = program.getInfoLog();
    if (status) {
        _help += info_log;
    } else {
        _reason = info_log;
    }

    return status;
}

bool ProgramBuilder::validate_program(gl::Program& program) {
    program.validate();

    GLint status = program.isValidateSucceed();
    std::string info_log = program.getInfoLog();

    if (status) {
        _help += info_log;
    } else {
        _reason = info_log;
    }

    return status;
}