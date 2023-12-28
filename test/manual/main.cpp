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
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"
#include "image/ImageLoaderStb.hpp"
#include "material/pbr/MaterialFactory_PBR.hpp"
#include "mesh/3d/QuadMesh_3D.hpp"
#include "shader/ProgramBuilder.hpp"
#include "transform/3d/TransformFactory_3D.hpp"
#include "view/Camera.hpp"

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

Ref<gl::Program> buildProgram() {
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

    if (not program) {
        std::cout << "ProgramBuilder: " << program_builder.reason().c_str() << std::endl;
        throw std::runtime_error("Failed init program");
    }

    if (program_builder.help().size() > 0) {
        std::cout << program_builder.help().c_str() << std::endl;
    }

    return program;
}

Ref<Material_PBR> createMaterial(MaterialFactory_PBR& factory,
                                 ImageLoaderStb& loader,
                                 const std::filesystem::path& albedo) {
    auto material = factory.createMaterial();
    auto albedo_tex = factory.createTex(MaterialTexType_PBR::Albedo);

    ImageInfo read_info{.format = ImageFormat::RGBA, .type = ImageType::UNSIGNED_BYTE};
    ImageInfo store_info{.width = 32,
                         .height = 32,
                         .format = ImageFormat::RGBA,
                         .type = ImageType::UNSIGNED_BYTE};

    auto result = loader.load(albedo, read_info, store_info);

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

    std::cout << material->getGLdata() << std::endl;

    return material;
}

// Ref<Mesh> createMesh() {}

int main(int argc, const char** argv) {
    auto current_dir = std::filesystem::current_path();

    bool close_window = false;
    GlfwWindow wnd(1024, 768);

    Ref<gl::Program> program = buildProgram();
    program->use();

    // Materials
    MaterialFactory_PBR material_factory(1, 32, 32, 10);
    ImageLoaderStb loader;
    auto material_gravel =
        createMaterial(material_factory,
                       loader,
                       current_dir / "3rdparty" / "minecraft_textures" / "gravel.png");

    auto mats = Mat4x4Storage::MakeCoherent(1000);
    program->bindShaderStorageBlockBuffer("Mat", mats->getBuffer());

    Camera cam(mats);
    program->bindShaderUniformBlockBuffer("Camera", cam.getBuffer());
    cam.setPosition({3, 3, 3});
    cam.lookAt({0, 0, 0});

    auto mesh = QuadMesh_3D::MakeDynamic(1);
    mesh->bindAttribute(program->glGetAttribLocation("inPos"), 0);
    // mesh->bindAttribute(program->glGetAttribLocation("inUV"), 1);
    // mesh->bindAttribute(program->glGetAttribLocation("inIds"), 2);
    std::array quads{
        mesh->createQuad(),
        // mesh->createQuad(),
        // mesh->createQuad(),
        // mesh->createQuad(),
        // mesh->createQuad(),
        // mesh->createQuad()
    };
    quads[0]->setQuad({VertGLdata_3D{{-1, 0, -1}, {0, 0}, {0, 0, 0, 0}},
                       VertGLdata_3D{{-1, 0, 1}, {0, 1}, {0, 0, 0, 0}},
                       VertGLdata_3D{{1, 0, -1}, {1, 0}, {0, 0, 0, 0}},
                       VertGLdata_3D{{1, 0, 1}, {1, 1}, {0, 0, 0, 0}}});
    // quads[1]->setQuad({VertGLdata_3D{{0, -1, -1}, {0, 0}, {0, 0, 0, 0}},
    //                    VertGLdata_3D{{0, -1, 1}, {0, 1}, {0, 0, 0, 0}},
    //                    VertGLdata_3D{{0, 1, -1}, {1, 0}, {0, 0, 0, 0}},
    //                    VertGLdata_3D{{0, 1, 1}, {1, 1}, {0, 0, 0, 0}}});
    // quads[2]->setQuad({VertGLdata_3D{{-1, 0, -1}, {0, 0}, {0, 0, 0, 0}},
    //                    VertGLdata_3D{{-1, 0, 1}, {0, 1}, {0, 0, 0, 0}},
    //                    VertGLdata_3D{{1, 0, -1}, {1, 0}, {0, 0, 0, 0}},
    //                    VertGLdata_3D{{1, 0, 1}, {1, 1}, {0, 0, 0, 0}}});

    wnd.setKeyCB([&close_window,
                  &cam](GlfwWindow& window, int key, int scancode, int action, int mods) {
        static constexpr float speed = 0.1;

        // std::cout << key << std::endl;
        std::cout << glm::to_string(cam.getPosition()) << std::endl;
        std::cout << glm::to_string(cam.getDirection()) << std::endl;
        std::cout << glm::to_string(cam.getViewMat()) << std::endl;
        switch (key) {
        case GLFW_KEY_ESCAPE:
            close_window = true;
            return;
        case GLFW_KEY_W:
            cam.setPosition(cam.getPosition() + speed * cam.getDirection());
            return;
        case GLFW_KEY_S:
            cam.setPosition(cam.getPosition() - speed * cam.getDirection());
            return;
        case GLFW_KEY_D:
            cam.setPosition(cam.getPosition() +
                            speed * glm::cross(cam.getDirection(), cam.getTop()));
            return;
        case GLFW_KEY_A:
            cam.setPosition(cam.getPosition() -
                            speed * glm::cross(cam.getDirection(), cam.getTop()));
            return;
        case GLFW_KEY_LEFT_CONTROL:
            cam.setPosition(cam.getPosition() - speed * cam.getTop());
            return;
        case GLFW_KEY_LEFT_SHIFT:
            cam.setPosition(cam.getPosition() + speed * cam.getTop());
            return;
        case GLFW_KEY_UP:
            cam.setDirection(glm::rotate(cam.getDirection(),
                                         glm::pi<float>() / 36,
                                         glm::cross(cam.getDirection(), cam.getTop())));
            return;
        case GLFW_KEY_DOWN:
            cam.setDirection(glm::rotate(cam.getDirection(),
                                         -glm::pi<float>() / 36,
                                         glm::cross(cam.getDirection(), cam.getTop())));
            return;
        case GLFW_KEY_LEFT:
            cam.setDirection(glm::rotate(cam.getDirection(),
                                         glm::pi<float>() / 36,
                                         cam.getTop()));
            return;
        case GLFW_KEY_RIGHT:
            cam.setDirection(glm::rotate(cam.getDirection(),
                                         -glm::pi<float>() / 36,
                                         cam.getTop()));
            return;
        default:
            return;
        }
    });

    gl::Ctx::Get().glClearColor(0, 0, 0.2f, 1.f);
    while (not close_window) {
        wnd.pollEvents();
        wnd.swapBuffers();

        gl::Ctx::Get().glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mesh->draw();
    }

    return 0;
}