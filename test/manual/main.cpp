
#include <memory>
#include <optional>
#include <stdexcept>

#include "gl/texture/TexImage.hpp"
#include "material/image_loader/MaterialImageLoaderStb.hpp"
#include "material/pbr/MaterialDataStorage_PBR.hpp"
#include "material/pbr/MaterialGL_PBR.hpp"
#include "material/pbr/MaterialStorage_PBR.hpp"
#include "material/pbr/MaterialTexStorage_PBR.hpp"
#include "material/pbr/Material_PBR.hpp"

#ifdef WIN32
#include <vld.h>
#endif

#include <filesystem>
#include <fstream>
#include <iostream>

#include "GlfwWindow.hpp"
#include "gl/Ctx.hpp"
#include "gl/Program.hpp"
// #include "material/image_loader/MaterialImageLoaderStb.hpp"
#include "material/pbr/MaterialFactory_PBR.hpp"
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

gnev::MaterialFactory_PBR buildMaterialFactory() {
    static constexpr GLuint CAP = 10;
    static constexpr GLuint WIDTH = 64;
    static constexpr GLuint HEIGHT = 64;

    auto data_storage =
        std::make_shared<gnev::MaterialDataStorage_PBR>(GL_DYNAMIC_STORAGE_BIT,
                                                        CAP,
                                                        std::nullopt);
    // clang-format off
    std::array<const std::shared_ptr<gnev::MaterialTexStorage_PBR>, 5> tex_storages = {
        std::make_shared<gnev::MaterialTexStorage_PBR>(CAP, 1, WIDTH, HEIGHT, GL_RGBA8, std::nullopt),
        std::make_shared<gnev::MaterialTexStorage_PBR>(CAP, 1, WIDTH, HEIGHT, GL_RGBA8, std::nullopt),
        std::make_shared<gnev::MaterialTexStorage_PBR>(CAP, 1, WIDTH, HEIGHT, GL_RGBA8, std::nullopt),
        std::make_shared<gnev::MaterialTexStorage_PBR>(CAP, 1, WIDTH, HEIGHT, GL_RGBA8, std::nullopt),
        std::make_shared<gnev::MaterialTexStorage_PBR>(CAP, 1, WIDTH, HEIGHT, GL_RGBA8, std::nullopt),
    };
    // clang-format on
    auto storage =
        std::make_shared<gnev::MaterialStorage_PBR>(data_storage, tex_storages);
    return gnev::MaterialFactory_PBR(storage);
}

gnev::Material_PBR createMaterial(gnev::MaterialFactory_PBR& factory) {
    auto material = factory.create();
    std::cout << "DataIndex: " << *material.getDataRef().getIndex() << std::endl;

    gnev::MaterialImageLoaderStb loader;
    auto current_dir = std::filesystem::current_path();
    gnev::gl::TexImageInfo info{.level = 0,
                                .x = 0,
                                .y = 0,
                                .width = 64,
                                .height = 64,
                                .format = GL_RGB,
                                .type = GL_UNSIGNED_BYTE};

    auto result =
        material.loadTex(gnev::MaterialTexType_PBR::Albedo,
                         loader,
                         current_dir / "3rdparty" / "minecraft_textures" / "gravel.png",
                         info);
    if (not result->done.get()) {
        throw std::runtime_error("");
    }
    std::cout << "TexIndex: " << *result->tex_ref.getIndex() << std::endl;

    material.setTexOffset(gnev::MaterialTexType_PBR::Albedo, {0.2, 0.2, 0.2, 0.2});
    material.setTexMultiplier(gnev::MaterialTexType_PBR::Albedo, {0.1, 0.1, 0.1, 0.1});

    auto data_iter = material.lockStorage()->data_storage->at(0);
    std::cout << *data_iter;

    return material;
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
    std::array<std::unique_ptr<gnev::Material_PBR>, 10> materials;
    for (int i = 0; i < 10; ++i) {
        materials[i] =
            std::make_unique<gnev::Material_PBR>(createMaterial(material_factory));
    }

    while (not close_window) {
        wnd.pollEvents();
        wnd.swapBuffers();
    }

    return 0;
}