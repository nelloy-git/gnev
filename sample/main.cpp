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

const std::string& get_vertex_shader_src(){
    static const std::string src = R"(
#version 430 core

layout (std140) uniform Camera {
    mat4 camera_mat;
};

in vec3 inPos;
in vec3 inOffset;
in vec4 inColor;

out vec4 vPos;
out vec4 vColor;

void main(){
    vPos = camera_mat * vec4(inPos + inOffset, 1.0);
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
    fColor = vColor;
} 
    )";
    return src;
}

static constexpr gnev::AttribInfo AttribPos(3, GL_FLOAT, false);
static constexpr gnev::AttribInfo AttribColor(4, GL_UNSIGNED_BYTE, true);

using PrimitiveVertex = gnev::Vertex<AttribPos>;
using InstanceVertex = gnev::Vertex<AttribPos, AttribColor>;

const PrimitiveVertex prim_vert[] =
{
    {{0, 1, 0}},
    {{0, 1, 1}},
    {{1, 1, 0}},
    {{1, 1, 1}},

    {{0, 0, 0}},
    {{0, 0, 1}},
    {{1, 0, 0}},
    {{1, 0, 1}},
};

const InstanceVertex inst_vert[] = {
    {{0, 0, 0}, {255, 0, 0, 255}},
    {{1, 1, 0}, {255, 0, 0, 255}},
    {{1, 0, 0}, {255, 0, 0, 255}},
    {{1, 0, 0}, {255, 0, 0, 255}},
    {{1, 0, 0}, {255, 0, 0, 255}},
    {{1, 0, 0}, {255, 0, 0, 255}},
    {{1, 0, 0}, {255, 0, 0, 255}},
    {{1, 0, 0}, {255, 0, 0, 255}},
    
    {{0, 1, 0}, {255, 255, 0, 255}},
    {{0, 1, 0}, {255, 0, 0, 255}},
    {{0, 1, 0}, {255, 0, 0, 255}},
    {{0, 1, 0}, {255, 0, 0, 255}},
    {{0, 1, 0}, {255, 0, 0, 255}},
    {{0, 1, 0}, {255, 0, 0, 255}},
    {{0, 1, 0}, {255, 0, 0, 255}},
    {{0, 1, 0}, {255, 0, 0, 255}},
};

const GLubyte prim_ind[] = {
    // top
    0, 3, 2,
    0, 1, 3,
    // bottom
    4, 7, 5,
    4, 6, 7,
    // front
    6, 2, 3,
    6, 3, 7,
    // back
    4, 1, 0,
    4, 5, 1,
    // right
    4, 2, 6,
    4, 0, 2,
    // left
    5, 3, 1,
    5, 7, 3,
};

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

        gnev::GLBufferArrayT<GLubyte> prim_ibo(drawer.ctx, prim_ind, sizeof(prim_ind) / sizeof(GLubyte), 0);
        gnev::GLBufferArrayT<PrimitiveVertex> prim_vbo(drawer.ctx, prim_vert, sizeof(prim_vert) / sizeof(PrimitiveVertex), GL_MAP_READ_BIT);
        gnev::GLBufferVectorT<InstanceVertex> inst_vbo(drawer.ctx, inst_vert, sizeof(inst_vert) / sizeof(InstanceVertex), GL_DYNAMIC_DRAW);

        gnev::Chunk chunk(drawer.ctx, prim_ibo, prim_vbo, inst_vbo);

        chunk.bind_primitive(drawer.program.glGetAttribLocation("inPos"), 0);

        chunk.bind_instance(drawer.program.glGetAttribLocation("inOffset"), 0);
        chunk.bind_instance(drawer.program.glGetAttribLocation("inColor"), 1);


        gnev::PlateChunk<InstanceVertex> ch(drawer.ctx);
        ch.bind<0>(drawer.program.glGetAttribLocation("inPos"));
        ch.bind<1>(drawer.program.glGetAttribLocation("inColor"));

        ch.add({
            InstanceVertex{{0.0f, 0.0f, 0.0f}, {0, 0, 255, 255}},
            InstanceVertex{{2.0f, 0.0f, 0.0f}, {0, 0, 255, 255}},
            InstanceVertex{{2.0f, 2.0f, 0.0f}, {0, 0, 255, 255}},
            InstanceVertex{{0.0f, 2.0f, 0.0f}, {0, 0, 255, 255}}
        });


        auto last_print = std::chrono::steady_clock::now();
        std::queue<std::chrono::microseconds> times;

        while(alive){
            conveyor.poll_events();

            auto start = std::chrono::steady_clock::now();
            conveyor.swap_buffers();
            drawer.draw();
            chunk.draw();
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