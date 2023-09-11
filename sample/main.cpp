#ifdef WIN32
#include <vld.h>
#endif

#include <chrono>
#include <iostream>
#include <fstream>
#include <queue>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GlfwConveyor.hpp"

#include "CameraController.hpp"
#include "Vertex.hpp"

#include "Drawer.hpp"
#include "shader/ProgramBuilder.hpp"

#include "material/MaterialFactory.hpp"

#include "voxel/VoxelChunk.hpp"
#include "VoxelTypeTest.hpp"

void read_text_file(std::string& dst, const std::filesystem::path& path)
{
    std::cout << "Loading shader " << path.string().c_str() << std::endl;
    if (!std::filesystem::exists(path)){
        throw std::runtime_error("File not found");
    }

    std::ifstream t(path);
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

gnev::gl::BufferVector<PointLight> create_lights(const std::shared_ptr<GladGLContext>& ctx)
{
    gnev::gl::BufferVector<PointLight> buffer(ctx, 0, nullptr);

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

GLint shader_storage_block_index()
{
    static GLint i = 0;
    return i++;
}

int main(int argc, const char** argv)
{
    GlfwConveyor conveyor(640, 480);
    gnev::Drawer drawer(conveyor.get_proc_address());
    gnev::ProgramBuilder program_builder(drawer.ctx);
   
    bool alive = true;
    bool show_polygons = false;

    auto exit_cb = [&](GlfwConveyor* conveyor, int key, int scancode, int action, int mods){
        if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE){
            alive = false;
        }

        if (key == GLFW_KEY_P && action == GLFW_PRESS){
            drawer.ctx->PolygonMode(GL_FRONT_AND_BACK, show_polygons ? GL_LINE : GL_FILL);
            show_polygons = !show_polygons;
        }
    };
    conveyor.key_callbacks.emplace_back(exit_cb);

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
    auto camera_buffer_index = drawer.program.glGetProgramResourceIndex(GL_SHADER_STORAGE_BLOCK, "CameraBuffer");
    auto camera_buffer_binding = shader_storage_block_index();
    drawer.program.glShaderStorageBlockBinding(camera_buffer_index, camera_buffer_binding);
    camera.buffer().glBindBufferBase(GL_SHADER_STORAGE_BUFFER, camera_buffer_binding);

    // glfwSetInputMode(conveyor.window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetInputMode(conveyor.window.get(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

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

    auto voxel_type = std::make_shared<VoxelTypeTest>(gravel_id);
    gnev::VoxelChunk<unsigned int, Vertex> chunk(drawer.ctx, 3, 3, 3);
    chunk.mesh().bind_attribute(drawer.program.glGetAttribLocation("inPos"), 0);
    chunk.mesh().bind_attribute(drawer.program.glGetAttribLocation("inUV"), 1);
    chunk.mesh().bind_attribute(drawer.program.glGetAttribLocation("inMaterialId"), 2);
    
    chunk.set(voxel_type, 0, 0, 0);
    chunk.set(voxel_type, 1, 0, 0);
    chunk.set(voxel_type, 1, 0, 1);
    chunk.set(voxel_type, 0, 0, 1);
    chunk.set(voxel_type, 0, 1, 0);
    // chunk.set(voxel_type, 1, 1, 0);
    // chunk.set(voxel_type, 1, 1, 1);
    // chunk.set(voxel_type, 0, 1, 1);
    // chunk.set(nullptr, 0, 1, 1);
    chunk.apply_mesh();
    
    // std::cout << "Indices:" << std::endl;
    // for (int i = 0; i < chunk.mesh().indices().size(); ++i){
    //     std::cout << "\t" << i << ": " << *chunk.mesh().indices().get(i) << std::endl;
    // }

    // std::cout << "Vertices: " << chunk.mesh().vertices().size() << std::endl;
    // for (int i = 0; i < chunk.mesh().vertices().size(); ++i){
    //     auto cur = chunk.mesh().vertices().get(i);
    //     std::cout << "\t" << i << ": "
    //         << "{" << cur->get<0>().data[0] << ", " << cur->get<0>().data[1] << ", " << cur->get<0>().data[2] << "}"
    //         << "{" << cur->get<1>().data[0] << ", " << cur->get<1>().data[1] << "}"
    //         << "{" << cur->get<2>().data[0] << "}"
    //         << std::endl;
    // }


    auto lights_buffer = create_lights(drawer.ctx);
    auto lights_buffer_index = drawer.program.glGetProgramResourceIndex(GL_SHADER_STORAGE_BLOCK, "PointLightBuffer");
    auto lights_buffer_binding = shader_storage_block_index();
    drawer.program.glShaderStorageBlockBinding(lights_buffer_index, lights_buffer_binding);
    lights_buffer.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, lights_buffer_binding);
    
    auto material_buffer_index = drawer.program.glGetProgramResourceIndex(GL_SHADER_STORAGE_BLOCK, "MaterialBuffer");
    auto material_buffer_binding = shader_storage_block_index();
    drawer.program.glShaderStorageBlockBinding(material_buffer_index, material_buffer_binding);
    material_factory.material_buffer().glBindBufferBase(GL_SHADER_STORAGE_BUFFER, material_buffer_binding);

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

        chunk.mesh().vao().glBindVertexArray();
        drawer.ctx->DrawElements(GL_TRIANGLES, chunk.mesh().indices().size(), chunk.mesh().IndexEnum, 0);

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

    return 0;
}