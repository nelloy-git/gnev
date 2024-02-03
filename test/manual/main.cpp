

#ifdef WIN32
#include <vld.h>
#endif

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>

#include "GlfwWindow.hpp"
#include "gl/Ctx.hpp"
#include "gl/Program.hpp"
#include "gl/Sampler.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"
#include "image/ImageLoaderStb.hpp"
#include "material/pbr/MaterialStorage_PBR.hpp"
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

Ref<base::ImageLoaderResult> loadImg(ImageLoaderStb& loader,
                                     const std::filesystem::path& path,
                                     const ImageInfo& store_info) {
    static constexpr ImageInfo read_info{.format = TextureFormat::RGBA,
                                         .type = TextureType::UNSIGNED_BYTE};

    auto result = loader.load(path, read_info, store_info);

    auto stbi_result_opt = result.dynamicCast<ImageLoaderStbResult>();
    if (not stbi_result_opt.has_value()) {
        throw std::runtime_error("");
    }
    std::cout << *stbi_result_opt.value() << std::endl;

    if (result->getStatus() != OperationStatus::Done) {
        throw std::runtime_error("");
    }

    for (int i = 0; i < 4; i++) {
        std::cout << int(result->image.data.buffer[i]) << " ";
    }
    std::cout << std::endl;

    return result;
}

Ref<Material_PBR>
createMaterial(const Ref<MaterialStorage_PBR>& storage,
               ImageLoaderStb& loader,
               const std::filesystem::path& albedo,
               const std::optional<std::filesystem::path>& normal = std::nullopt,
               const std::optional<std::filesystem::path>& specular = std::nullopt) {
    static constexpr ImageInfo store_info{.width = 128,
                                          .height = 128,
                                          .format = TextureFormat::RGBA,
                                          .type = TextureType::UNSIGNED_BYTE};

    auto material = MakeSharable<Material_PBR>(storage);

    auto albedo_tex = MakeSharable<
        Material_PBR::TexElem>(storage->getTexView(MaterialTexType_PBR::Albedo));
    albedo_tex->set(loadImg(loader, albedo, store_info)->image);
    material->setTex(MaterialTexType_PBR::Albedo, albedo_tex);

    if (normal.has_value()) {
        auto normal_tex = MakeSharable<
            Material_PBR::TexElem>(storage->getTexView(MaterialTexType_PBR::Normal));
        normal_tex->set(loadImg(loader, normal.value(), store_info)->image);
        material->setTex(MaterialTexType_PBR::Normal, normal_tex);
    }

    if (specular.has_value()) {
        auto specular_tex = MakeSharable<
            Material_PBR::TexElem>(storage->getTexView(MaterialTexType_PBR::Metallic));
        specular_tex->set(loadImg(loader, specular.value(), store_info)->image);
        material->setTex(MaterialTexType_PBR::Metallic, specular_tex);
    }

    // MaterialDataGL_PBR data_gl;
    // material->getData()->get(data_gl);
    // std::cout << data_gl << std::endl;

    return material;
}

struct PointLight {
    alignas(16) glm::vec3 pos = {0, 1, 0};
    alignas(16) glm::vec3 constant_linearic_quadratic = {1.0, 0.18, 0.032};
    alignas(16) glm::vec3 ambient = {1.0f, 1.0f, 1.0f};
    alignas(16) glm::vec3 diffuse = {1.0f, 1.0f, 1.0f};
    alignas(16) glm::vec3 specular = {1.0f, 1.0f, 1.0f};
};

struct Test {
    void log(const CtStrDef& str = getMethodName()) {
        std::cout << "HERE: " << str.to_string_view().data() << std::endl;
    }
};

int main(int argc, const char** argv) {
    auto current_dir = std::filesystem::current_path();

    bool close_window = false;
    GlfwWindow wnd(1024, 768);

    Ref<gl::Program> program = buildProgram();
    program->use();

    // Materials
    auto material_storage = MaterialStorage_PBR::MakeDynamic(1, 128, 128, 10);
    ImageLoaderStb loader;
    program
        ->bindShaderStorageBlockBuffer("Material",
                                       material_storage->getDataView()->accessor->buffer);
    program
        ->bindShaderTextureSampler("inTexAlbedo",
                                   material_storage->getTex(MaterialTexType_PBR::Albedo));
    program
        ->bindShaderTextureSampler("inTexNormal",
                                   material_storage->getTex(MaterialTexType_PBR::Normal));
    program->bindShaderTextureSampler("inTexSpecular",
                                      material_storage
                                          ->getTex(MaterialTexType_PBR::Metallic));

    // Sampler
    auto tex_sampler = gl::Sampler::MakeNearestRepeat();
    tex_sampler->bind(0);
    tex_sampler->bind(1);
    tex_sampler->bind(2);

    // Matrixes
    auto mats = Mat4x4Storage::MakeCoherent(1000);
    program->bindShaderStorageBlockBuffer("Mat", mats->getBuffer());

    // Lights
    auto lights = gl::buffer::IndexMapView<PointLight>::MakeDynamic(10);
    program->bindShaderStorageBlockBuffer("PointLight", lights->accessor->buffer);
    auto light = gl::buffer::WeakIndexMapViewElem<PointLight>(lights);

    // Camera
    Camera cam(mats);
    program->bindShaderUniformBlockBuffer("Camera", cam.getBuffer());
    cam.setPosition({3, 3, 3});
    cam.lookAt({0, 0, 0});

    auto mesh = QuadMesh_3D::MakeDynamic(1);
    mesh->bindAttribute(program->getAttributeLoc("inPos"), 0);
    mesh->bindAttribute(program->getAttributeLoc("inUV"), 1);
    mesh->bindAttribute(program->getAttributeLoc("inIds"), 2);
    std::array quads{
        mesh->createQuad(),
    };
    auto material_gravel =
        createMaterial(material_storage,
                       loader,
                       current_dir / "3rdparty" / "minecraft_textures" / "gravel.png");
    quads[0]->setQuad({VertGLdata_3D{{-1, 0, -1},
                                     {0, 0},
                                     {material_gravel->getData()->getIndex(), 0, 0, 0}},
                       VertGLdata_3D{{-1, 0, 1},
                                     {0, 1},
                                     {material_gravel->getData()->getIndex(), 0, 0, 0}},
                       VertGLdata_3D{{1, 0, -1},
                                     {1, 0},
                                     {material_gravel->getData()->getIndex(), 0, 0, 0}},
                       VertGLdata_3D{{1, 0, 1},
                                     {1, 1},
                                     {material_gravel->getData()->getIndex(), 0, 0, 0}}});

    wnd.setKeyCB([&close_window,
                  &cam](GlfwWindow& window, int key, int scancode, int action, int mods) {
        static constexpr float speed = 0.1;

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

    unsigned char pixel[6] = "\0\0\0\0\0";
    gl::Ctx::Get().glGetTextureSubImage(material_storage
                                            ->getTex(MaterialTexType_PBR::Albedo)
                                            ->handle(),
                                        0,
                                        0,
                                        0,
                                        0,
                                        1,
                                        1,
                                        1,
                                        GL_RGBA,
                                        GL_UNSIGNED_BYTE,
                                        4,
                                        pixel);

    gl::Ctx::Get().glClearColor(0, 0, 0.2f, 1.f);
    while (not close_window) {
        wnd.pollEvents();
        wnd.swapBuffers();

        gl::Ctx::Get().glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mesh->draw();
    }

    return 0;
}