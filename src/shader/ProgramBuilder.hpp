#pragma once

#include <optional>
#include <unordered_map>
#include <string>

#include "gl/GLShader.hpp"
#include "gl/GLProgram.hpp"

namespace gnev {

class EXPORT ProgramBuilder {
public:
    ProgramBuilder(const std::shared_ptr<GladGLContext>& ctx);
    ~ProgramBuilder();

    const std::string& reason() const;
    const std::string& help() const;
    std::optional<GLProgram> build(const std::unordered_map<GLenum, std::string>& sources);

private:
    const std::shared_ptr<GladGLContext> _ctx;
    std::string _reason;
    std::string _help;

    bool compile_shader(GLShader& shader, const std::string& src);
    bool link_program(GLProgram& program);
    bool validate_program(GLProgram& program);
};

}