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

static constexpr gnev::AttribInfo AttribPos(3, GL_FLOAT, false);
static constexpr gnev::AttribInfo AttribColor(4, GL_UNSIGNED_BYTE, true);



using Vertex = gnev::Vertex<AttribPos, AttribColor>;
using a0 = Vertex::attrib_type<0>::type;
using a1 = Vertex::attrib_type<1>::type;

const std::string& get_vertex_shader_src(){
    static const std::string src = R"(
#version 430 core

layout (std140) uniform Camera {
    mat4 camera_mat;
};

in vec3 inPos;
in vec4 inColor;

out vec4 vPos;
out vec4 vColor;

void main(){
    vPos = camera_mat * vec4(inPos, 1.0);
    vColor = inColor;
    gl_Position = vPos;
}
    )";
    return src;
}

const std::string& get_fragment_shader_src(){
    static const std::string src = R"(
#version 430 core

uniform sampler2DArray texture_array_0;

in vec4 vColor;

out vec4 fColor;

void main(){
    fColor = vec4(1,1,1,1);
} 
    )";
    return src;
}

const gnev::Attrib<AttribPos> pos = {0.0F, 1.0F, 0.0F};

const Vertex prim_vert[] = {
    {{0, 1, 0}, {1.0F, 1.0F, 1.0F, 1.0F}},
    {{0, 1, 1}, {1.0F, 1.0F, 1.0F, 1.0F}},
    {{1, 1, 0}, {1.0F, 1.0F, 1.0F, 1.0F}},
    {{1, 1, 1}, {1.0F, 1.0F, 1.0F, 1.0F}}
};

const GLuint prim_ind[] = {
    0, 2, 3,
    0, 3, 1
};

int main(int argc, const char** argv) {
    GlfwConveyor conveyor;
    conveyor.worker.push([&conveyor](){
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

        // using v = Vertex::type

        gnev::Chunk<Vertex, Vertex> chunk(drawer.ctx, prim_ind, 6, reinterpret_cast<const Vertex*>(prim_vert), 4);
        chunk.bind_primitive<0>(drawer.program.glGetAttribLocation("inPos"));

        auto& p = chunk._prim_vbo;
        for (int i = 0; i < p.size(); ++i){
            auto v = p.get(i);
            std::cout << "v[" << i << "]: "
                      << "{"
                      << v->get_attrib<0>().data[0] << " "
                      << v->get_attrib<0>().data[1] << " "
                      << v->get_attrib<0>().data[2]
                      << "}"
                      << "{"
                      << int(v->get_attrib<1>().data[0]) << " "
                      << int(v->get_attrib<1>().data[1]) << " "
                      << int(v->get_attrib<1>().data[2]) << " "
                      << int(v->get_attrib<1>().data[3])
                      << "}"
                      << std::endl;
        }

        // chunk.bind_primitive<1>(drawer.program.glGetAttribLocation("inColor"));

        // gnev::GLVertexArray vao(drawer.ctx);
        
        // gnev::GLBufferVectorT<GLuint> ibo(drawer.ctx, GL_DYNAMIC_COPY);
        // gnev::GLBufferVectorT<GLfloat[3]> vbo(drawer.ctx, GL_DYNAMIC_COPY);

        // gnev::GLBufferVectorT<GLuint> t(gnev::GLBufferVector(drawer.ctx, nullptr, 12, GL_DYNAMIC_DRAW));
        // gnev::GLBufferVector t2 = t;
        
        // init_vertices(ibo, vbo);
        // init_vao(drawer.program, vao, ibo, vbo);

        bool alive = true;
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

        auto last_print = std::chrono::steady_clock::now();
        std::queue<std::chrono::microseconds> times;

        while(alive){
            conveyor.poll_events();

            auto start = std::chrono::steady_clock::now();
            conveyor.swap_buffers();
            drawer.draw();
            // vao.glBindVertexArray();
            // drawer.ctx->DrawElements(GL_TRIANGLES, ibo.size(), GL_UNSIGNED_INT, 0);

            chunk._vao.glBindVertexArray();
            drawer.ctx->DrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1);

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