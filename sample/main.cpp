#ifdef WIN32
#include <vld.h>
#endif

#include <chrono>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GlfwConveyor.hpp"

#include "gl/GLBuffer.hpp"
#include "gl/GLVertexArray.hpp"

#include "Drawer.hpp"
#include "shader/ProgramBuilder.hpp"

#include "data/Vertex.hpp"
#include "gl/GLBufferVectorT.hpp"

#include "data/Chunk.hpp"
#include "data/PlateChunk.hpp"
#include "material/MaterialFactory.hpp"

const std::string& get_vertex_shader_src(){
    static const std::string src = R"(
#version 430 core

layout (std140) uniform Camera {
    mat4 camera_mat;
    vec3 view_pos;
    vec3 view_dir;
};

in vec3 inPos;
in vec2 inUV;
in int inMaterialId;

out vec4 vPos;
out vec2 vUV;
flat out int vMaterialId;

void main(){
    vPos = camera_mat * vec4(inPos, 1.0);
    vUV = inUV;
    vMaterialId = inMaterialId;
    gl_Position = vPos;
}
    )";
    return src;
}

const std::string& get_fragment_shader_src(){
    static const std::string src = R"(
#version 430 core

struct MaterialTexture {
    int diffuse_array_index;
    int diffuse_element_index;
    int normal_array_index;
    int normal_element_index;
    int specular_array_index;
    int specular_element_index;
};

struct Material
{
    vec4 color;
    vec4 normal;
    vec4 specular;
    MaterialTexture texture;
};

layout (std430) buffer MaterialBuffer
{
    Material materials[];
};

layout (std140) uniform Camera {
    mat4 camera_mat;
    vec3 view_pos;
    vec3 view_dir;
};

struct PointLight { 
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (std430) buffer PointLightBuffer
{
    PointLight lights[];
};

uniform sampler2DArray diffuse_0;
// uniform sampler2DArray diffuse_1;
uniform sampler2DArray normal_0;
// uniform sampler2DArray normal_1;
uniform sampler2DArray specular_0;
// uniform sampler2DArray specular_1;

in vec4 vPos;
in vec2 vUV;
flat in int vMaterialId;

out vec4 fColor;

vec3 CalcPointLight(PointLight light, Material material, vec3 fragPos, vec3 viewDir)
{
    vec3 diffuse = texture(diffuse_0, vec3(vUV, material.texture.diffuse_element_index)).rgb;
    vec3 normal = texture(normal_0, vec3(vUV, material.texture.normal_element_index)).rgb;
    vec3 specular = texture(specular_0, vec3(vUV, material.texture.specular_element_index)).rgb;

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = 100;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient_light  = light.ambient  * diffuse;
    vec3 diffuse_light  = light.diffuse  * diff * diffuse;
    vec3 specular_light = light.specular * spec * specular;
    ambient_light  *= attenuation;
    diffuse_light  *= attenuation;
    specular_light *= attenuation;
    return (ambient_light + diffuse_light + specular_light);
} 

void main(){
    Material material = materials[vMaterialId];
    fColor = vec4(CalcPointLight(lights[0], material, vPos.rgb, view_dir), 1);
} 
    
)";
    return src;
}

static constexpr gnev::AttribInfo AttribPos(3, GL_FLOAT, false);
static constexpr gnev::AttribInfo AttribUV(2, GL_FLOAT, false);
static constexpr gnev::AttribInfo AttribMaterialID(1, GL_INT, false);

using Vertex = gnev::Vertex<AttribPos, AttribUV, AttribMaterialID>;

void bind_camera_controll(GlfwConveyor& conveyor, gnev::Drawer& drawer, bool& alive)
{
    conveyor.key_callback = [&alive, &drawer](GlfwConveyor* conveyor, int key, int scancode, int action, int mods){
        const float vel = 0.05;

        if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE){
            alive = false;
        }

        if (action == GLFW_PRESS || action == GLFW_REPEAT){
            bool moved = false;
            switch (key){
            case GLFW_KEY_A: drawer.camera.pos.x += vel; moved = true; break;
            case GLFW_KEY_D: drawer.camera.pos.x -= vel; moved = true; break;
            case GLFW_KEY_LEFT_CONTROL: drawer.camera.pos.y += vel; moved = true; break;
            case GLFW_KEY_SPACE: drawer.camera.pos.y -= vel; moved = true; break;
            case GLFW_KEY_W: drawer.camera.pos.z += vel; moved = true; break;
            case GLFW_KEY_S: drawer.camera.pos.z -= vel; moved = true; break;
            default: break;
            }
            if (moved){
                drawer.camera.applyLookAt({0,0,0});
            }
        }
    };
}

struct PointLight {
    float pos[3];

    float constant;
    float linearic;
    float quadratic;

    float ambient[3];
    float diffuse[3];
    float specular[3];
};

gnev::GLBufferVectorT<PointLight> create_lights(const std::shared_ptr<GladGLContext>& ctx)
{
    gnev::GLBufferVectorT<PointLight> buffer(ctx, nullptr, 0, GL_DYNAMIC_DRAW);

    PointLight light = {
        .pos = {0, 1, 0},
        .constant = 1.0f,
        .linearic = 0.18f,
        .quadratic = 0.032f,
        .ambient = {1, 1, 1},
        .diffuse = {1, 1, 1},
        .specular = {1, 1, 1}
    };
    buffer.push_back(light);

    return buffer;
}

int main(int argc, const char** argv)
{
    GlfwConveyor conveyor;
    conveyor.worker.push([&conveyor](){
        bool alive = true;

        gnev::Drawer drawer(conveyor.get_proc_address());
        gnev::ProgramBuilder program_builder(drawer.ctx);

        auto program = program_builder.build({
            {GL_VERTEX_SHADER, get_vertex_shader_src()},
            {GL_FRAGMENT_SHADER, get_fragment_shader_src()},
        });
        if (program.has_value()){
            drawer.program = program.value();
        } else {
            std::cout << "ProgramBuilder: " << program_builder.reason().c_str() << std::endl;
            throw std::runtime_error("Failed init program");
        }
        if (program_builder.help().size() > 0){
            std::cout << program_builder.help().c_str() << std::endl;
        }
        bind_camera_controll(conveyor, drawer, alive);

        // Material

        gnev::MaterialFactory material_factory(drawer.ctx,
                                               4, 128, 128,
                                               4, 128, 128,
                                               4, 128, 128);
        gnev::MaterialInfo gravel_info = {
            .texture = {
                .diffuse_path = "D:\\projects\\gnev\\3rdparty\\minecraft_textures\\gravel.png",
                .normal_path = "D:\\projects\\gnev\\3rdparty\\minecraft_textures\\gravel_n.png",
                .specular_path = "D:\\projects\\gnev\\3rdparty\\minecraft_textures\\gravel_s.png"
            }
        };
        auto gravel_id = material_factory.register_material(L"gravel", gravel_info);

        // Plate

        gnev::PlateChunk<Vertex> ch(drawer.ctx);
        ch.bind<0>(drawer.program.glGetAttribLocation("inPos"));
        ch.bind<1>(drawer.program.glGetAttribLocation("inUV"));
        ch.bind<2>(drawer.program.glGetAttribLocation("inMaterialId"));

        {
            Vertex v0{{0.0f, 0.0f, 0.0f}, {0, 0}, {gravel_id}};
            Vertex v1{{2.0f, 0.0f, 0.0f}, {2, 0}, {gravel_id}};
            Vertex v2{{2.0f, 2.0f, 0.0f}, {2, 2}, {gravel_id}};
            Vertex v3{{0.0f, 2.0f, 0.0f}, {0, 2}, {gravel_id}};
            ch.add({v0, v1, v2, v3});
        }
        {
            Vertex v0{{0.0f, 0.0f, 2.0f}, {0, 2}, {gravel_id}};
            Vertex v1{{0.0f, 0.0f, 0.0f}, {0, 0}, {gravel_id}};
            Vertex v2{{0.0f, 2.0f, 0.0f}, {2, 0}, {gravel_id}};
            Vertex v3{{0.0f, 2.0f, 2.0f}, {2, 2}, {gravel_id}};
            ch.add({v0, v1, v2, v3});
        }

        auto lights_buffer = create_lights(drawer.ctx);
        auto lights_buffer_index = drawer.program.glGetProgramResourceIndex(GL_SHADER_STORAGE_BLOCK, "PointLightBuffer");
        drawer.program.glShaderStorageBlockBinding(lights_buffer_index, 0);
        lights_buffer.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0);
        
        auto material_buffer_index = drawer.program.glGetProgramResourceIndex(GL_SHADER_STORAGE_BLOCK, "MaterialBuffer");
        drawer.program.glShaderStorageBlockBinding(material_buffer_index, 1);
        material_factory.material_buffer().glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1);

        auto last_print = std::chrono::steady_clock::now();
        std::queue<std::chrono::microseconds> times;
        while(alive){
            conveyor.poll_events();

            auto start = std::chrono::steady_clock::now();
            conveyor.swap_buffers();
            drawer.draw();

            drawer.ctx->ActiveTexture(GL_TEXTURE0);
            drawer.sampler.glBindSampler(0);
            drawer.ctx->Uniform1i(drawer.program.glGetUniformLocation("diffuse_0"), 0);
            material_factory.diffuse_loader().textures()[0].glBindTexture(GL_TEXTURE_2D_ARRAY);
            
            drawer.ctx->ActiveTexture(GL_TEXTURE1);
            drawer.sampler.glBindSampler(1);
            drawer.ctx->Uniform1i(drawer.program.glGetUniformLocation("normal_0"), 1);
            material_factory.normal_loader().textures()[0].glBindTexture(GL_TEXTURE_2D_ARRAY);
            
            drawer.ctx->ActiveTexture(GL_TEXTURE2);
            drawer.sampler.glBindSampler(2);
            drawer.ctx->Uniform1i(drawer.program.glGetUniformLocation("specular_0"), 2);
            material_factory.specular_loader().textures()[0].glBindTexture(GL_TEXTURE_2D_ARRAY);

            ch.draw();
            // test_vao.glBindVertexArray();
            // drawer.ctx->DrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1);

            // chunk._vao.glBindVertexArray();
            // drawer.ctx->DrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0, 1);

            // drawer.ctx->DrawArrays(GL_TRIANGLES, 0, 3);
            auto end = std::chrono::steady_clock::now();
            auto dt = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            times.push(dt);

            if (std::chrono::duration_cast<std::chrono::seconds>(end - last_print).count() > 0){
                size_t count = times.size();
                size_t sum = 0;
                while (times.size() > 0){
                    sum += times.front().count();
                    times.pop();
                }
                std::cout << "Average time: " << sum / count << "us" << std::endl;
                last_print = std::chrono::steady_clock::now();
            }
        }
    });

    return 0;
}