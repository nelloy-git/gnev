#pragma once

#include <memory>

#include "gl/Program.hpp"
#include "gl/Shader.hpp"
#include "gl/enum/ShaderType.hpp"
#include "magic_enum/magic_enum_containers.hpp"

namespace gnev::gl {

class Pipeline {
public:
    bool setShader(ShaderType type, const GLchar* const source, GLint length);

    unsigned getAttribLoc(const std::string_view& name) const;

    bool build();
    void use() const;

private:
    Program program;
    magic_enum::containers::array<ShaderType, std::shared_ptr<Shader>> active_shader;

};

} // namespace gnev::gl