#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "gl/Program.hpp"
#include "gl/Shader.hpp"

namespace gnev {

class EXPORT ProgramBuilder {
public:
    ProgramBuilder();
    ~ProgramBuilder();

    const std::string& reason() const;
    const std::string& help() const;
    std::optional<gl::Program> build(const std::unordered_map<GLenum, std::string>& sources);

private:
    std::string _reason;
    std::string _help;

    bool compile_shader(gl::Shader& shader, const std::string& src);
    bool link_program(gl::Program& program);
    bool validate_program(gl::Program& program);
};

} // namespace gnev