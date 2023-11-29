#include <memory>
#include <optional>
#include <stdexcept>

#include "magic_enum.hpp"
#include "util/StrongRef.hpp"

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
#include "material/pbr/Alias.hpp"
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

MaterialFactory_PBR buildMaterialFactory() {
    static constexpr GLuint CAP = 10;
    static constexpr GLuint WIDTH = 64;
    static constexpr GLuint HEIGHT = 64;

    auto data_storage = StrongRef<MaterialDataStorage_PBR>::Make(CAP);

    std::array<StrongRef<MaterialTexStorage_PBR>, 5> tex_storages = {
        StrongRef<MaterialTexStorage_PBR>::Make(1, GL_RGBA8, WIDTH, HEIGHT, CAP),
        StrongRef<MaterialTexStorage_PBR>::Make(1, GL_RGBA8, WIDTH, HEIGHT, CAP),
        StrongRef<MaterialTexStorage_PBR>::Make(1, GL_RGBA8, WIDTH, HEIGHT, CAP),
        StrongRef<MaterialTexStorage_PBR>::Make(1, GL_RGBA8, WIDTH, HEIGHT, CAP),
        StrongRef<MaterialTexStorage_PBR>::Make(1, GL_RGBA8, WIDTH, HEIGHT, CAP),
    };

    auto storage = StrongRef<MaterialStorage_PBR>::Make(data_storage, tex_storages);
    return MaterialFactory_PBR(storage);
}

std::unique_ptr<Material_PBR>
createMaterial(MaterialFactory_PBR& factory, MaterialImageLoaderStbi& loader) {
    //

    auto material = std::make_unique<Material_PBR>(factory.create());
    auto current_dir = std::filesystem::current_path();
    gl::TexImageInfo info{.level = 0,
                          .x = 0,
                          .y = 0,
                          .width = 32,
                          .height = 32,
                          .format = GL_RGBA,
                          .type = GL_UNSIGNED_BYTE};

    StrongRef<MaterialTex_PBR>
        tex_ref(material->getWeakStorage().lock().value()->textures.at(0));
        
    auto result =
        loader.upload(tex_ref,
                      current_dir / "3rdparty" / "minecraft_textures" / "gravel.png",
                      info,
                      info);

    auto full_result_opt = result.dynamicCast<MaterialImageLoaderStbiResult>();
    if (not full_result_opt.has_value()) {
        throw std::runtime_error("");
    }

    auto& full_result = full_result_opt.value();
    std::cout << "Stb msgs: [";
    if (full_result->messages.size() > 0) {
        std::cout << magic_enum::enum_name(full_result->messages[0]);
        for (int i = 1; i < full_result->messages.size(); ++i) {
            std::cout << ", " << magic_enum::enum_name(full_result->messages[i]);
        }
    }
    std::cout << "]" << std::endl;

    result->done.wait();
    if (not full_result->done.get()) {
        throw std::runtime_error("");
    }

    // sizeof(MaterialGL_PBR);
    material->setTexRef(MaterialTexType_PBR::Albedo, tex_ref);
    material->setTexOffset(MaterialTexType_PBR::Albedo,
                           {material->getDataRef()->getIndex() * 0.2,
                            material->getDataRef()->getIndex() * 0.2,
                            material->getDataRef()->getIndex() * 0.2,
                            material->getDataRef()->getIndex() * 0.2});
    material->setTexMultiplier(MaterialTexType_PBR::Albedo,
                               {material->getDataRef()->getIndex() * 0.1,
                                material->getDataRef()->getIndex() * 0.1,
                                material->getDataRef()->getIndex() * 0.1,
                                material->getDataRef()->getIndex() * 0.1});

    auto data = std::make_unique<MaterialGL_PBR>();
    material->getDataRef()->template getData<MaterialGL_PBR>(data.get(), 0);
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
    auto material_factory = buildMaterialFactory();
    MaterialImageLoaderStbi loader;
    std::array<std::unique_ptr<Material_PBR>, 10> materials;
    for (int i = 0; i < 10; ++i) {
        materials[i] = createMaterial(material_factory, loader);
    }

    while (not close_window) {
        wnd.pollEvents();
        wnd.swapBuffers();
    }

    return 0;
}