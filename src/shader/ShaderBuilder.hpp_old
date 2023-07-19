#pragma once

#include <string>

#include "gl/GLShader.hpp"
#include "gl/GLProgram.hpp"

namespace gnev {

class ShaderBuilder final {
public:
    ShaderBuilder(const std::shared_ptr<GladGLContext>& ctx);
    ~ShaderBuilder();

    std::unique_ptr<GLShader> make_vertex_shader();
    std::unique_ptr<GLShader> make_geometry_shader();
    std::unique_ptr<GLShader> make_fragment_shader();
    std::unique_ptr<GLProgram> make_program();

    enum class Input {
        Pos_vec3,
    };

    const GLchar* const get_input_name(Input input) const;

    std::string reason;

private:
    std::shared_ptr<GladGLContext> _ctx;

    static const std::string& get_vertex_shader_src();
    static const std::string& get_geometry_shader_src();
    static const std::string& get_fragment_shader_src();

};

}