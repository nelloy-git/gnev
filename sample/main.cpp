#ifdef WIN32
#include <vld.h>
#endif

#include <chrono>
#include <iostream>
#include <fstream>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GlfwConveyor.hpp"
#include "CameraController.hpp"

#include "gl/GLBuffer.hpp"
#include "gl/GLVertexArray.hpp"

#include "Drawer.hpp"
#include "shader/ProgramBuilder.hpp"

#include "data/Vertex.hpp"
#include "gl/GLBufferVectorT.hpp"

#include "data/Chunk.hpp"
#include "data/PlateChunk.hpp"
#include "material/MaterialFactory.hpp"

static constexpr gnev::AttribInfo AttribPos(3, GL_FLOAT, false);
static constexpr gnev::AttribInfo AttribUV(2, GL_FLOAT, false);
static constexpr gnev::AttribInfo AttribMaterialID(1, GL_INT, false);

using Vertex = gnev::Vertex<AttribPos, AttribUV, AttribMaterialID>;

void read_text_file(std::string& dst, const std::filesystem::path& path)
{
    std::cout << "Loading shader " << path.string().c_str() << std::endl;
    if (!std::filesystem::exists(path)){
        throw std::runtime_error("File not found");
    }

    std::ifstream t(path.wstring());
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    dst.resize(size, ' ');
    t.seekg(0);
    t.read(dst.data(), size);
}

struct PointLight {
    GLfloat pos[4];

    GLfloat constant;
    GLfloat linearic;
    GLfloat quadratic;
    GLfloat reserve;

    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
};

gnev::GLBufferVectorT<PointLight> create_lights(const std::shared_ptr<GladGLContext>& ctx)
{
    gnev::GLBufferVectorT<PointLight> buffer(ctx, nullptr, 0, GL_DYNAMIC_DRAW);

    PointLight light = {
        .pos = {0, 1, 0},
        .constant = 1.0f,
        .linearic = 0.18f,
        .quadratic = 0.032f,
        .ambient = {1.0f, 1.0f, 1.0f},
        .diffuse = {1.0f, 1.0f, 1.0f},
        .specular = {1.0f, 1.0f, 1.0f}
    };
    buffer.push_back(light);

    return buffer;
}

int main(int argc, const char** argv)
{
    GlfwConveyor conveyor;
   
    conveyor.worker.push([&conveyor](){
        bool alive = true;

        auto exit_cb = [&alive](GlfwConveyor* conveyor, int key, int scancode, int action, int mods){
            if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE){
                alive = false;
            }
        };
        conveyor.key_callbacks.push_back(exit_cb);
        sizeof(exit_cb);


        gnev::Drawer drawer(conveyor.get_proc_address());
        gnev::ProgramBuilder program_builder(drawer.ctx);

        auto current_dir = std::filesystem::current_path();
        std::string vertex_shader_src;
        read_text_file(vertex_shader_src, current_dir / "sample" / "shader" / "vertex.vs");
        std::string fragment_shader_src;
        read_text_file(fragment_shader_src, current_dir / "sample" / "shader" / "fragment.fs");

        auto program = program_builder.build({
            {GL_VERTEX_SHADER, vertex_shader_src},
            {GL_FRAGMENT_SHADER, fragment_shader_src},
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

        CameraController camera(drawer.ctx);
        camera.capture(conveyor);
        drawer.program.glUniformBlockBinding(drawer.program.glGetUniformBlockIndex("Camera"), 0);
        camera.buffer().glBindBufferBase(GL_UNIFORM_BUFFER, 0);
        glfwSetInputMode(conveyor.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

        // Material

        gnev::MaterialFactory material_factory(drawer.ctx,
                                               4, 128, 128,
                                               4, 128, 128,
                                               4, 128, 128);
        auto diffuse_path = current_dir / "3rdparty/minecraft_textures/gravel.png";
        auto normal_path = current_dir / "3rdparty/minecraft_textures/gravel_n.png";
        auto specular_path = current_dir / "3rdparty/minecraft_textures/gravel_s.png";
        gnev::MaterialInfo gravel_info = {
            .texture = {
                .diffuse_path = diffuse_path,
                .normal_path = normal_path,
                .specular_path = specular_path
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