#include <memory>
#include <optional>
#include <stdexcept>

#include "magic_enum.hpp"
#include "util/Ref.hpp"

#ifdef WIN32
#include <vld.h>
#endif

#include <filesystem>
#include <fstream>
#include <iostream>

#include "GlfwWindow.hpp"
#include "gl/Ctx.hpp"
#include "gl/Program.hpp"
#include "material/image_loader/MaterialImageLoaderStbi.hpp"
#include "material/pbr/MaterialFactory_PBR.hpp"
#include "shader/ProgramBuilder.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

using namespace gnev;

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

gl::Program buildProgram() {
    ProgramBuilder program_builder;

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

Ref<Material_PBR>
createMaterial(MaterialFactory_PBR& factory, MaterialImageLoaderStbi& loader) {
    //

    auto material = factory.createMaterial();
    auto albedo_tex = factory.createTex(MaterialTexType_PBR::Albedo);

    auto current_dir = std::filesystem::current_path();
    gl::TexImageInfo info{.level = 0,
                          .x = 0,
                          .y = 0,
                          .width = 32,
                          .height = 32,
                          .format = GL_RGBA,
                          .type = GL_UNSIGNED_BYTE};

    auto result =
        loader.upload(albedo_tex,
                      current_dir / "3rdparty" / "minecraft_textures" / "gravel.png",
                      info,
                      info);

    result->done.wait();
    if (not result->done.get()) {
        throw std::runtime_error("");
    }

    auto stbi_result_opt = result.dynamicCast<MaterialImageLoaderStbiResult>();
    if (not stbi_result_opt.has_value()) {
        throw std::runtime_error("");
    }
    std::cout << *stbi_result_opt.value() << std::endl;

    material->setTexRef(MaterialTexType_PBR::Albedo, albedo_tex);

    auto data = std::make_unique<MaterialGL_PBR>();
    material->getDataRef()->getData<MaterialGL_PBR>(data.get(), 0);
    std::cout << *data << std::endl;

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
    MaterialFactory_PBR material_factory(1, 32, 32, 10);
    MaterialImageLoaderStbi loader;
    std::vector<Ref<Material_PBR>> materials;
    for (int i = 0; i < 10; ++i) {
        materials.emplace_back(createMaterial(material_factory, loader));
    }

    while (not close_window) {
        wnd.pollEvents();
        wnd.swapBuffers();
    }

    return 0;
}