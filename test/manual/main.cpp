
#include <optional>

#include "material/pbr/MaterialDataPBR.hpp"
#ifdef WIN32
#include <vld.h>
#endif

#include <filesystem>
#include <fstream>
#include <iostream>

#include "GlfwWindow.hpp"
#include "gl/Ctx.hpp"
#include "gl/Program.hpp"
#include "material/image_loader/MaterialImageLoaderStb.hpp"
#include "material/pbr/MaterialFactoryPBR.hpp"
#include "shader/ProgramBuilder.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

void readTextFile(std::string& dst, const std::filesystem::path& path) {
    std::cout << "Loading shader " << path.string().c_str() << std::endl;
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("File not found");
    }

    std::ifstream t(path);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    dst.resize(size, ' ');
    t.seekg(0);
    t.read(dst.data(), size);
}

gnev::gl::Program buildProgram() {
    gnev::ProgramBuilder program_builder;

    auto current_dir = std::filesystem::current_path();
    std::string vertex_shader_src;
    readTextFile(vertex_shader_src, current_dir / "sample" / "shader" / "vertex.vs");
    std::string fragment_shader_src;
    readTextFile(fragment_shader_src, current_dir / "sample" / "shader" / "fragment.fs");

    auto program = program_builder.build({
        {GL_VERTEX_SHADER, vertex_shader_src},
        {GL_FRAGMENT_SHADER, fragment_shader_src},
    });

    if (not program.has_value()) {
        std::cout << "ProgramBuilder: " << program_builder.reason().c_str() << std::endl;
        throw std::runtime_error("Failed init program");
    }

    if (program_builder.help().size() > 0) {
        std::cout << program_builder.help().c_str() << std::endl;
    }

    return std::move(program.value());
}

gnev::MaterialFactoryPBR buildMaterialFactory() {
    static constexpr GLuint CAP = 10;

    gnev::MaterialFactoryPBR::DataStorageSettings data_settings{
        .capacity = 10,
        .storage_flags = GL_DYNAMIC_STORAGE_BIT,
        .clean_up = std::nullopt};

    gnev::MaterialFactoryPBR::TexStorageSettings albedo_settings{
        .tex_i = 0,
        .capacity = CAP,
        .levels = 1,
        .width = 64,
        .height = 64,
        .internal_format = GL_RGBA8,
        .clean_up = std::nullopt,
        .loader = std::make_shared<gnev::MaterialImageLoaderStb>()};

    // gnev::MaterialFactoryPBR::TexStorageSettings normal_settings{
    //     .tex_i = 1,
    //     .capacity = CAP,
    //     .levels = 1,
    //     .width = 64,
    //     .height = 64,
    //     .internal_format = GL_RGBA8,
    //     .clean_up = std::nullopt};

    // gnev::MaterialFactoryPBR::TexStorageSettings metallic_settings{
    //     .tex_i = 2,
    //     .capacity = CAP,
    //     .levels = 1,
    //     .width = 64,
    //     .height = 64,
    //     .internal_format = GL_RGBA8,
    //     .clean_up = std::nullopt};

    // gnev::MaterialFactoryPBR::TexStorageSettings roughness_settings{
    //     .tex_i = 3,
    //     .capacity = CAP,
    //     .levels = 1,
    //     .width = 64,
    //     .height = 64,
    //     .internal_format = GL_RGBA8,
    //     .clean_up = std::nullopt};

    // gnev::MaterialFactoryPBR::TexStorageSettings ambient_occlusion_settings{
    //     .tex_i = 4,
    //     .capacity = CAP,
    //     .levels = 1,
    //     .width = 64,
    //     .height = 64,
    //     .internal_format = GL_RGBA8,
    //     .clean_up = std::nullopt};

    auto factory = gnev::MaterialFactoryPBR(data_settings,
                                            {
                                                albedo_settings,
                                                //  normal_settings,
                                                //  metallic_settings,
                                                //  roughness_settings,
                                                //  ambient_occlusion_settings
                                            });
    return factory;
}

int main(int argc, const char** argv) {
    bool close_window = false;
    GlfwWindow wnd(1024, 768);

    wnd.setKeyCB([&close_window](GlfwWindow& window,
                                 int key,
                                 int scancode,
                                 int action,
                                 int mods) {
        std::cout << key << std::endl;
        if (key == GLFW_KEY_ESCAPE) {
            close_window = true;
        }
    });

    auto program = buildProgram();
    auto material_factory = buildMaterialFactory();
    auto material = material_factory.create();

    auto current_dir = std::filesystem::current_path();
    material.uploadTexture(gnev::ParamPBR::Albedo,
                           current_dir / "3rdparty" / "minecraft_textures" / "gravel.png",
                           0);

    while (not close_window) {
        wnd.pollEvents();
        wnd.swapBuffers();
    }

    return 0;
}