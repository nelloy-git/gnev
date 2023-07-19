#include "ProgramBuilder.hpp"

#include <iostream>

using namespace gnev;

ProgramBuilder::ProgramBuilder(const std::shared_ptr<GladGLContext>& ctx) :
    _ctx(ctx)
{   
}

ProgramBuilder::~ProgramBuilder()
{
}

const std::string& ProgramBuilder::reason() const 
{
    return _reason;
}

const std::string& ProgramBuilder::help() const 
{
    return _help;
}

std::optional<GLProgram> ProgramBuilder::build(const std::unordered_map<GLenum, std::string>& sources)
{
    _reason = "";
    _help = "";

    GLProgram program(_ctx);
    for (auto& shader_info : sources){
        GLShader shader(_ctx, shader_info.first);
        auto shader_status = compile_shader(shader, shader_info.second);
        if (!shader_status){
            return std::nullopt;
        }
        program.glAttachShader(shader.handle());
    }

    auto link_status = link_program(program);
    if (!link_status){
        return std::nullopt;
    }

    auto validate_status = validate_program(program);
    if (!validate_status){
        return std::nullopt;
    }

    return program;
}

bool ProgramBuilder::compile_shader(GLShader& shader, const std::string& src)
{
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

    if (status){
        _help += dst;
    } else {
        _reason = dst;
    }

    return status;
}

bool ProgramBuilder::link_program(GLProgram& program)
{
    program.glLinkProgram();

    GLint status;
    program.glGetProgramiv(GL_LINK_STATUS, &status);

    GLint len;
    program.glGetProgramiv(GL_INFO_LOG_LENGTH, &len);

    std::string dst;
    dst.resize(len);
    program.glGetProgramInfoLog(len, &len, dst.data());

    if (status){
        _help += dst;
    } else {
        _reason = dst;
    }

    return status;
}

bool ProgramBuilder::validate_program(GLProgram& program)
{
    program.glValidateProgram();

    GLint status;
    program.glGetProgramiv(GL_VALIDATE_STATUS, &status);

    GLint len;
    program.glGetProgramiv(GL_INFO_LOG_LENGTH, &len);

    std::string dst;
    dst.resize(len);
    program.glGetProgramInfoLog(len, &len, dst.data());
    
    if (status){
        _help += dst;
    } else {
        _reason = dst;
    }

    return status;
}