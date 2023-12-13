#include <memory>
#include <optional>
#include <stdexcept>

#include "magic_enum/magic_enum.hpp"
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
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/transform.hpp"
#include "image/ImageLoaderStb.hpp"
#include "material/pbr/MaterialFactory_PBR.hpp"
#include "shader/ProgramBuilder.hpp"
#include "transform/3d/TransformFactory_3D.hpp"

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

Ref<Material_PBR> createMaterial(MaterialFactory_PBR& factory, ImageLoaderStb& loader) {
    //

    auto material = factory.createMaterial();
    auto albedo_tex = factory.createTex(MaterialTexType_PBR::Albedo);

    auto current_dir = std::filesystem::current_path();
    ImageInfo read_info{.format = ImageFormat::RGBA, .type = ImageType::UNSIGNED_BYTE};
    ImageInfo store_info{.width = 32,
                         .height = 32,
                         .format = ImageFormat::RGBA,
                         .type = ImageType::UNSIGNED_BYTE};

    auto result =
        loader.load(current_dir / "3rdparty" / "minecraft_textures" / "gravel.png",
                    read_info,
                    store_info);

    auto stbi_result_opt = result.dynamicCast<ImageLoaderStbResult>();
    if (not stbi_result_opt.has_value()) {
        throw std::runtime_error("");
    }
    std::cout << *stbi_result_opt.value() << std::endl;

    if (result->getStatus() != OperationStatus::Done) {
        throw std::runtime_error("");
    }

    albedo_tex->set(result->image);
    material->setTex(MaterialTexType_PBR::Albedo, albedo_tex);
    material->setTexOffset(MaterialTexType_PBR::Normal,
                           glm::vec4(albedo_tex->getIndex()));
    material->changeTexOffset(MaterialTexType_PBR::Normal,
                              [](glm::vec4& val) { val /= 10; });
    material->setTexMultiplier(MaterialTexType_PBR::Metallic,
                               glm::vec4(1.0 - albedo_tex->getIndex()));

    std::cout << material->get() << std::endl;

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

    // Materials
    MaterialFactory_PBR material_factory(1, 32, 32, 10);
    ImageLoaderStb loader;
    std::vector<Ref<Material_PBR>> materials;
    for (int i = 0; i < 10; ++i) {
        materials.emplace_back(createMaterial(material_factory, loader));
    }

    // Transforms
    TransformFactory_3D transform_factory(10);
    std::vector<Ref<Transform_3D>> transforms;
    for (int i = 0; i < 10; ++i) {
        auto tr = transform_factory.createTransform();

        tr->setPosition({i, 0, 0});
        if (transforms.size() > 0) {
            tr->setParent(transforms.back());
        }

        std::cout << tr->get() << std::endl;

        transforms.emplace_back(tr);
    }

    while (not close_window) {
        wnd.pollEvents();
        wnd.swapBuffers();
    }

    return 0;
}