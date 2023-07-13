#include "shader/ShaderBuilder.hpp"

using namespace gnev;

ShaderBuilder::ShaderBuilder(const std::shared_ptr<GladGLContext>& ctx) :
    _ctx(ctx){
}

ShaderBuilder::~ShaderBuilder(){
}

std::unique_ptr<GLShader> ShaderBuilder::make_vertex_shader(){
    auto vertex_shader = std::make_unique<GLShader>(_ctx, GL_VERTEX_SHADER);
    if (!vertex_shader){
        reason = "VertexShader: init failed";
        return nullptr;
    }

    auto compile_result = vertex_shader->compile_with_info_log(get_vertex_shader_src());
    if (!compile_result.first){
        reason = "VertexShader:\n" + compile_result.second;
        return nullptr;
    }

    return vertex_shader;
}

std::unique_ptr<GLShader> ShaderBuilder::make_geometry_shader(){
    auto geometry_shader = std::make_unique<GLShader>(_ctx, GL_GEOMETRY_SHADER);
    if (!geometry_shader){
        reason = "GeometryShader: init failed";
        return nullptr;
    }

    auto compile_result = geometry_shader->compile_with_info_log(get_geometry_shader_src());
    if (!compile_result.first){
        reason = "GeometryShader:\n" + compile_result.second;
        return nullptr;
    }

    return geometry_shader;
}

std::unique_ptr<GLShader> ShaderBuilder::make_fragment_shader(){
    auto fragment_shader = std::make_unique<GLShader>(_ctx, GL_FRAGMENT_SHADER);
    if (!fragment_shader){
        reason = "FragmentShader: init failed";
        return nullptr;
    }

    auto compile_result = fragment_shader->compile_with_info_log(get_fragment_shader_src());
    if (!compile_result.first){
        reason = "FragmentShader:\n" + compile_result.second;
        return nullptr;
    }

    return fragment_shader;
}

std::unique_ptr<GLProgram> ShaderBuilder::make_program(){
    // Vertex shader
    auto vertex_shader = make_vertex_shader();
    if (!vertex_shader){
        return nullptr;
    }

    // Geometry shader
    auto geometry_shader = make_geometry_shader();
    if (!geometry_shader){
        return nullptr;
    }

    // Fragment shader
    auto fragment_shader = make_fragment_shader();
    if (!fragment_shader){
        return nullptr;
    }

    // Program
    auto program = std::make_unique<GLProgram>(_ctx);
    if (!program){
        reason = "Program: init failed";
        return nullptr;
    }
    program->glAttachShader(vertex_shader->handle);
    program->glAttachShader(geometry_shader->handle);
    program->glAttachShader(fragment_shader->handle);
    auto link_result = program->link_with_info_log();
    if (!link_result.first){
        reason = "Program:\n" + link_result.second;
        return nullptr;
    }
    auto validate_result = program->validate_with_info_log();
    if (!validate_result.first){
        reason = "Program:\n" + validate_result.second;
        return nullptr;
    }

    return std::move(program);
}

const GLchar* const ShaderBuilder::get_input_name(Input input) const {
    switch (input){
        using enum Input;
        case Pos_vec3: return "inPos";
        default: return "Uknown Input type";
    }
}

const std::string& ShaderBuilder::get_vertex_shader_src(){
    static const std::string src = R"(
#version 410 core

in vec3 inPos;

out vec4 vPos;

void main(){
    vPos = vec4(inPos, 1.0);
}
    )";
    return src;
}

const std::string& ShaderBuilder::get_geometry_shader_src(){
    static const std::string src = R"(
#version 410 core
layout (points) in;
layout (triangle_strip, max_vertices = 18) out;

layout (std140) uniform Camera {
    mat4 camera_mat;
};

in vec4 vPos[1];

out vec4 gPos;
out vec2 gTex;
out vec4 gColor;
flat out int gSide;

void emit_vertex(in vec4 vert_offset, in vec2 tex_coord, in vec4 color, in int side){
    gl_Position = camera_mat * (vPos[0]  + vert_offset);
    gTex = tex_coord;
    gColor = color;
    gSide = side;
    EmitVertex();
}

void emit_cube(){
    emit_vertex(vec4(0, 0, 0, 0), vec2(0, 0), vec4(0.2, 0.2, 0.2, 1), 0);
    emit_vertex(vec4(2, 0, 0, 0), vec2(2, 0), vec4(0.2, 0.2, 0.2, 1), 0);
    emit_vertex(vec4(0, 0, 2, 0), vec2(0, 2), vec4(0.2, 0.2, 0.2, 1), 0);
    EndPrimitive();
    
    emit_vertex(vec4(0, 1, 0, 0), vec2(0, 0), vec4(1, 1, 1, 1), 2);
    emit_vertex(vec4(0, 1, 2, 0), vec2(2, 0), vec4(1, 1, 1, 1), 2);
    emit_vertex(vec4(2, 1, 0, 0), vec2(0, 2), vec4(1, 1, 1, 1), 2);
    EndPrimitive();

    emit_vertex(vec4(0, 0, 0, 0), vec2(0, 0), vec4(0.8, 0.8, 0.8, 1), 1);
    emit_vertex(vec4(0, 0, 2, 0), vec2(2, 0), vec4(0.8, 0.8, 0.8, 1), 1);
    emit_vertex(vec4(0, 2, 0, 0), vec2(0, 2), vec4(0.8, 0.8, 0.8, 1), 1);
    EndPrimitive();

    emit_vertex(vec4(0, 0, 0, 0), vec2(0, 0), vec4(0.7, 0.7, 0.7, 1), 1);
    emit_vertex(vec4(0, 2, 0, 0), vec2(0, 2), vec4(0.7, 0.7, 0.7, 1), 1);
    emit_vertex(vec4(2, 0, 0, 0), vec2(2, 0), vec4(0.7, 0.7, 0.7, 1), 1);
    EndPrimitive();

    emit_vertex(vec4(1, 0,  1, 0), vec2(0, 0), vec4(0.6, 0.6, 0.6, 1), 1);
    emit_vertex(vec4(1, 0, -1, 0), vec2(2, 0), vec4(0.6, 0.6, 0.6, 1), 1);
    emit_vertex(vec4(1, 2,  1, 0), vec2(0, 2), vec4(0.6, 0.6, 0.6, 1), 1);
    EndPrimitive();

    emit_vertex(vec4( 1, 0, 1, 0), vec2(0, 0), vec4(0.5, 0.5, 0.5, 1), 1);
    emit_vertex(vec4( 1, 2, 1, 0), vec2(0, 2), vec4(0.5, 0.5, 0.5, 1), 1);
    emit_vertex(vec4(-1, 0, 1, 0), vec2(2, 0), vec4(0.5, 0.5, 0.5, 1), 1);
    EndPrimitive();
}

void main() {
    emit_cube();
}  
    )";
    return src;
}

const std::string& ShaderBuilder::get_fragment_shader_src(){
    static const std::string src = R"(
#version 410 core

uniform sampler2DArray texture_array_0;
uniform sampler2DArray texture_array_1;
uniform sampler2DArray texture_array_2;
uniform sampler2DArray texture_array_3;
uniform sampler2DArray texture_array_4;
uniform sampler2DArray texture_array_5;
uniform sampler2DArray texture_array_6;
uniform sampler2DArray texture_array_7;
uniform sampler2DArray texture_array_8;
uniform sampler2DArray texture_array_9;
uniform sampler2DArray texture_array_10;
uniform sampler2DArray texture_array_11;
uniform sampler2DArray texture_array_12;
uniform sampler2DArray texture_array_13;
uniform sampler2DArray texture_array_14;
uniform sampler2DArray texture_array_15;

in vec2 gTex;
in vec4 gColor;
flat in int gSide;

out vec4 fColor;

void main(){
    vec4 color = gColor * texture(texture_array_0, vec3(gTex, gSide));
    if (color.a < 0.05){
        discard;
    }
    fColor = color;
}
    )";
    return src;
}