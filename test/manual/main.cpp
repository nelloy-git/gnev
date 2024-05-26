#include <type_traits>

#include "gl/data/VertexAttribute.hpp"
#include "gl/enum/AttributeType.hpp"
#include "gl/enum/ShaderType.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "util/Reflection.hpp"
#ifdef WIN32
#include <vld.h>
#endif

#include <filesystem>
#include <memory>

#include "GlfwWindow.hpp"
#include "Mat4x4.hpp"
#include "gl/container/BufferAccessorMapped.hpp"
#include "gl/container/BufferAllocatorStorage.hpp"
#include "gl/container/BufferPool.hpp"
#include "gl/container/BufferPoolElement.hpp"
#include "gl/data/Vertex.hpp"
#include "gl/data/VertexAttributeInfo.hpp"
#include "gl/enum/BufferMapRangeAccess.hpp"
#include "gl/enum/BufferStorageFlags.hpp"
#include "gl/pipeline/Pipeline.hpp"

using namespace gnev;

static constexpr gl::VertexAttributeInfo VERTEX_ATTR_0{
    2, gl::AttributeType::HALF_FLOAT, false};
static constexpr gl::VertexAttributeInfo VERTEX_ATTR_1{4, gl::AttributeType::BYTE, false};
static constexpr gl::VertexAttributeInfo VERTEX_ATTR_2{4, gl::AttributeType::INT, false};

std::string readTextFile(const std::filesystem::path& path) {
    std::cout << "Loading shader " << path.string().c_str() << std::endl;
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("File not found");
    }

    std::ifstream t(path);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string dst(size, ' ');
    t.seekg(0);
    t.read(dst.data(), size);
    return dst;
}

// Ref<gl::Program> buildProgram() {
//     ProgramBuilder program_builder;

//     auto current_dir = std::filesystem::current_path();
//     std::string vertex_shader_src;
//     readTextFile(vertex_shader_src, current_dir / "sample" / "shader" / "vertex.vs");
//     std::string fragment_shader_src;
//     readTextFile(fragment_shader_src, current_dir / "sample" / "shader" /
//     "fragment.fs");

//     auto program = program_builder.build({
//         {gl::ShaderType::VERTEX_SHADER, vertex_shader_src},
//         {gl::ShaderType::FRAGMENT_SHADER, fragment_shader_src},
//     });

//     if (not program) {
//         std::cout << "ProgramBuilder: " << program_builder.reason().c_str() <<
//         std::endl; throw std::runtime_error("Failed init program");
//     }

//     if (program_builder.help().size() > 0) {
//         std::cout << program_builder.help().c_str() << std::endl;
//     }

//     return program;
// }

// Ref<base::ImageLoaderResult> loadImg(ImageLoaderStb& loader,
//                                      const std::filesystem::path& path,
//                                      const ImageInfo& store_info) {
//     static constexpr ImageInfo read_info{.format = gl::TextureFormat::RGBA,
//                                          .type = gl::TextureType::UNSIGNED_BYTE};

//     auto result = loader.load(path, read_info, store_info);

//     auto stbi_result_opt = result.dynamicCast<ImageLoaderStbResult>();
//     if (not stbi_result_opt.has_value()) {
//         throw std::runtime_error("");
//     }
//     std::cout << *stbi_result_opt.value() << std::endl;

//     if (result->getStatus() != OperationStatus::Done) {
//         throw std::runtime_error("");
//     }

//     for (int i = 0; i < 4; i++) {
//         std::cout << int(result->image.data.buffer[i]) << " ";
//     }
//     std::cout << std::endl;

//     return result;
// }

// MaterialPbr
// createMaterial(MaterialPbrStorage& storage,
//                ImageLoaderStb& loader,
//                const std::filesystem::path& albedo,
//                const std::optional<std::filesystem::path>& normal = std::nullopt,
//                const std::optional<std::filesystem::path>& specular = std::nullopt) {
//     static constexpr ImageInfo store_info{.width = 64,
//                                           .height = 64,
//                                           .format = TextureFormat::RGBA,
//                                           .type = TextureType::UNSIGNED_BYTE};

//     auto material_opt = storage.create();
//     if (not material_opt.has_value()) {
//         throw std::runtime_error("No space");
//     }
//     auto material = material_opt.value();

//     auto albedo_tex = MakeSharable<
//         gl::texture::WeakIndexMapViewElem>(storage
//                                                .getTexView(MaterialPbr::TexType::Albedo));
//     albedo_tex->set(loadImg(loader, albedo, store_info)->image);
//     material.setTex(MaterialPbr::TexType::Albedo, albedo_tex);

//     if (normal.has_value()) {
//         auto normal_tex = MakeSharable<
//             gl::texture::WeakIndexMapViewElem>(storage.getTexView(MaterialPbr::TexType::
//                                                                       Normal));
//         normal_tex->set(loadImg(loader, normal.value(), store_info)->image);
//         material.setTex(MaterialPbr::TexType::Normal, normal_tex);
//     }

//     if (specular.has_value()) {
//         auto specular_tex = MakeSharable<
//             gl::texture::WeakIndexMapViewElem>(storage.getTexView(MaterialPbr::TexType::
//                                                                       Metallic));
//         specular_tex->set(loadImg(loader, specular.value(), store_info)->image);
//         material.setTex(MaterialPbr::TexType::Metallic, specular_tex);
//     }

//     return material;
// }

// struct PointLight {
//     alignas(16) glm::vec3 pos = {0, 1, 0};
//     alignas(16) glm::vec3 constant_linearic_quadratic = {1.0, 0.18, 0.032};
//     alignas(16) glm::vec3 ambient = {1.0f, 1.0f, 1.0f};
//     alignas(16) glm::vec3 diffuse = {1.0f, 1.0f, 1.0f};
//     alignas(16) glm::vec3 specular = {1.0f, 1.0f, 1.0f};
// };

std::shared_ptr<quill::Handler> initLoggerFileHandler(const quill::fs::path& path,
                                                      quill::LogLevel level) {
    auto handler = quill::file_handler(path, []() {
        quill::FileHandlerConfig cfg;
        cfg.set_open_mode('w');
        return cfg;
    }());
    handler->set_pattern("%(ascii_time) [%(thread)] %(logger_name:<9) %(level_name:<12) "
                         "%(message)",              // format
                         "%H:%M:%S.%Qms",           // timestamp format
                         quill::Timezone::GmtTime); // timestamp's timezone
    handler->set_log_level(level);
    return handler;
}

quill::Logger* initLogger() {
    auto logger =
        quill::create_logger("gnev",
                             initLoggerFileHandler("gnev.log", quill::LogLevel::TraceL3));
    logger->set_log_level(quill::LogLevel::TraceL2);
    return logger;
}

// struct Camera {
//     static constexpr unsigned int InvalidIndex = std::numeric_limits<unsigned
//     int>::max();

//     struct MatIndexes {
//         glm::uint view = InvalidIndex;
//         glm::uint proj = InvalidIndex;
//     };

//     refl::Aligned<glm::vec3, 16> position = {{0, 0, 0}};
//     refl::Aligned<glm::vec3, 16> direction = {{1, 0, 0}};
//     refl::Aligned<glm::vec3, 16> top = {{0, 1, 0}};
//     refl::Aligned<MatIndexes, 16> mats;
// };

template <typename T>
std::shared_ptr<gnev::gl::BufferPool<T>> makeBufferPoolMapped() {
    using namespace gnev::gl;
    auto accessor = std::make_unique<
        gnev::gl::BufferAccessorMapped>(BufferMapRangeAccess::MAP_READ_BIT |
                                        BufferMapRangeAccess::MAP_WRITE_BIT |
                                        BufferMapRangeAccess::MAP_PERSISTENT_BIT |
                                        BufferMapRangeAccess::MAP_COHERENT_BIT);
    auto allocator = std::make_unique<
        gnev::gl::BufferAllocatorStorage>(BufferStorageFlags::DYNAMIC_STORAGE_BIT |
                                          BufferStorageFlags::MAP_READ_BIT |
                                          BufferStorageFlags::MAP_WRITE_BIT |
                                          BufferStorageFlags::MAP_PERSISTENT_BIT |
                                          BufferStorageFlags::MAP_COHERENT_BIT);
    return std::make_shared<BufferPool<T>>(std::move(accessor), std::move(allocator), 1);
}

// GNEV_VERTEX(Vert, (VERTEX_ATTR_0)(attr_0), (VERTEX_ATTR_1)(attr_1));

struct Vert2 {
    gl::VertexAttribute<VERTEX_ATTR_0> attr_0;
    gl::VertexAttribute<VERTEX_ATTR_1> attr_1;
};

int main(int argc, const char** argv) {

    bool close_window = false;
    GlfwWindow wnd(1024, 768, initLogger());
    quill::start();

    auto mat4x4_storage = makeBufferPoolMapped<Mat4x4::GL>();

    // using Vert = gl::Vertex<VERTEX_INFO>;
    auto mesh_vert = makeBufferPoolMapped<Vert2>();
    auto v = mesh_vert->pull(1);
    mesh_vert->at(v.begin).set<"attr_0">({0, 2});

    gl::BufferPoolElement e{mesh_vert, {{0, 0}, {0, 0, 0, 0}}};
    e->set<"attr_1">({1, 1, 1, 1});

    auto current_dir = std::filesystem::current_path();
    auto vertex_shader_src =
        readTextFile(current_dir / "sample" / "shader" / "vertex_2d.vs");
    auto fragment_shader_src =
        readTextFile(current_dir / "sample" / "shader" / "fragment_2d.fs");

    gl::Pipeline pipeline;
    pipeline.setShader(gl::ShaderType::VERTEX_SHADER,
                       vertex_shader_src.data(),
                       vertex_shader_src.size());
    pipeline.setShader(gl::ShaderType::FRAGMENT_SHADER,
                       fragment_shader_src.data(),
                       fragment_shader_src.size());
    pipeline.build();

    // {
    //     auto buffer = std::make_unique<gl::Buffer>();
    //     buffer->initStorage(100 * sizeof(GlmMat4x4Meta),
    //                         nullptr,
    //                         gl::BufferStorageFlags::DYNAMIC_STORAGE_BIT);
    //     auto accessor =
    //     std::make_unique<gl::BufferRawAccessorSubData>(std::move(buffer)); auto
    //     refl_list =
    //         std::make_unique<gl::BufferReflList<GlmMat4x4Meta>>(std::move(accessor));

    //     auto range1 = refl_list->reserveRange(3);
    //     auto range2 = refl_list->reserveRange(2);

    //     auto v1 = refl_list->at(*range1);
    //     v1[0].set<GlmMat4x4Meta, "mat">(glm::mat4{1.f});
    //     v1[1].set<GlmMat4x4Meta, "mat">(glm::mat4{1.f});
    //     v1[2].set<GlmMat4x4Meta, "mat">(glm::mat4{1.f});

    //     auto v2 = refl_list->at(*range2);
    //     v2[0].set<GlmMat4x4Meta, "mat">(glm::mat4{1.f});
    //     v2[1].set<GlmMat4x4Meta, "mat">(glm::mat4{1.f});
    // }

    // using Nested = ReflStructDeduceMeta<TestReflStruct, 0, 0>;
    // using Child = DeduceReflStructChildFieldMeta<TestReflStruct, 0>;

    // Ref<gl::Program> program = buildProgram();
    // program->use();

    // // Materials
    // auto material_storage = MaterialPbrStorage(10, 1, 64, 64);
    // ImageLoaderStb loader;
    // program
    //     ->bindShaderStorageBlockBuffer("Material",
    //                                    material_storage->getDataView()->accessor->buffer);
    // program
    //     ->bindShaderTextureSampler("inTexAlbedo",
    //                                material_storage->getTex(MaterialTexType_PBR::Albedo));
    // program
    //     ->bindShaderTextureSampler("inTexNormal",
    //                                material_storage->getTex(MaterialTexType_PBR::Normal));
    // program->bindShaderTextureSampler("inTexSpecular",
    //                                   material_storage
    //                                       ->getTex(MaterialTexType_PBR::Metallic));

    // // Sampler
    // auto tex_sampler = gl::Sampler::MakeNearestRepeat();
    // tex_sampler->bind(0);
    // tex_sampler->bind(1);
    // tex_sampler->bind(2);

    // // Matrixes
    // auto mats = Mat4x4Storage::MakeCoherent(1000);
    // program->bindShaderStorageBlockBuffer("Mat", mats->getBuffer());

    // // Lights
    // auto lights = gl::buffer::IndexMapView<PointLight>::MakeDynamic(10);
    // program->bindShaderStorageBlockBuffer("PointLight", lights->accessor->buffer);
    // auto light = gl::buffer::WeakIndexMapViewElem<PointLight>(lights);

    // // Camera
    // Camera cam(mats);
    // program->bindShaderUniformBlockBuffer("Camera", cam.getBuffer());
    // cam.setPosition({3, 3, 3});
    // cam.lookAt({0, 0, 0});

    // auto mesh = QuadMesh_3D::MakeDynamic(1);
    // mesh->bindAttribute(program->getAttributeLoc("inPos"), 0);
    // mesh->bindAttribute(program->getAttributeLoc("inUV"), 1);
    // mesh->bindAttribute(program->getAttributeLoc("inIds"), 2);
    // std::array quads{
    //     mesh->createQuad(),
    // };
    // auto material_gravel =
    //     createMaterial(material_storage,
    //                    loader,
    //                    current_dir / "3rdparty" / "minecraft_textures" / "gravel.png");
    // quads[0]->setQuad({VertGLdata_3D{{-1, 0, -1},
    //                                  {0, 0},
    //                                  {material_gravel->getData()->getIndex(), 0, 0,
    //                                  0}},
    //                    VertGLdata_3D{{-1, 0, 1},
    //                                  {0, 1},
    //                                  {material_gravel->getData()->getIndex(), 0, 0,
    //                                  0}},
    //                    VertGLdata_3D{{1, 0, -1},
    //                                  {1, 0},
    //                                  {material_gravel->getData()->getIndex(), 0, 0,
    //                                  0}},
    //                    VertGLdata_3D{{1, 0, 1},
    //                                  {1, 1},
    //                                  {material_gravel->getData()->getIndex(), 0, 0,
    //                                  0}}});

    // auto new_mesh = Mesh<VertGLdata_3D>::MakeDynamicStorage(10);
    // new_mesh->bindAttribute(program->getAttributeLoc("inPos"), 0);
    // new_mesh->bindAttribute(program->getAttributeLoc("inUV"), 1);
    // new_mesh->bindAttribute(program->getAttributeLoc("inIds"), 2);

    // SubMesh<VertGLdata_3D> submesh{new_mesh, 6, 4};
    // submesh.setIndex(0, 0);
    // submesh.setIndex(1, 1);
    // submesh.setIndex(2, 2);
    // submesh.setIndex(3, 3);
    // submesh.setIndex(4, 2);
    // submesh.setIndex(5, 1);
    // submesh.setVertex(0,
    //                   VertGLdata_3D{{-1, 0, -1},
    //                                 {0, 0},
    //                                 {material_gravel->getData()->getIndex(), 0, 0,
    //                                 0}});
    // submesh.setVertex(1,
    //                   VertGLdata_3D{{-1, 0, 1},
    //                                 {0, 1},
    //                                 {material_gravel->getData()->getIndex(), 0, 0,
    //                                 0}});
    // submesh.setVertex(2,
    //                   VertGLdata_3D{{1, 0, -1},
    //                                 {1, 0},
    //                                 {material_gravel->getData()->getIndex(), 0, 0,
    //                                 0}});
    // submesh.setVertex(3,
    //                   VertGLdata_3D{{1, 0, 1},
    //                                 {1, 1},
    //                                 {material_gravel->getData()->getIndex(), 0, 0,
    //                                 0}});

    // wnd.setKeyCB([&close_window,
    //               &cam](GlfwWindow& window, int key, int scancode, int action, int
    //               mods) {
    //     static constexpr float speed = 0.1;

    //     switch (key) {
    //     case GLFW_KEY_ESCAPE:
    //         close_window = true;
    //         return;
    //     case GLFW_KEY_W:
    //         cam.setPosition(cam.getPosition() + speed * cam.getDirection());
    //         return;
    //     case GLFW_KEY_S:
    //         cam.setPosition(cam.getPosition() - speed * cam.getDirection());
    //         return;
    //     case GLFW_KEY_D:
    //         cam.setPosition(cam.getPosition() +
    //                         speed * glm::cross(cam.getDirection(), cam.getTop()));
    //         return;
    //     case GLFW_KEY_A:
    //         cam.setPosition(cam.getPosition() -
    //                         speed * glm::cross(cam.getDirection(), cam.getTop()));
    //         return;
    //     case GLFW_KEY_LEFT_CONTROL:
    //         cam.setPosition(cam.getPosition() - speed * cam.getTop());
    //         return;
    //     case GLFW_KEY_LEFT_SHIFT:
    //         cam.setPosition(cam.getPosition() + speed * cam.getTop());
    //         return;
    //     case GLFW_KEY_UP:
    //         cam.setDirection(glm::rotate(cam.getDirection(),
    //                                      glm::pi<float>() / 36,
    //                                      glm::cross(cam.getDirection(),
    //                                      cam.getTop())));
    //         return;
    //     case GLFW_KEY_DOWN:
    //         cam.setDirection(glm::rotate(cam.getDirection(),
    //                                      -glm::pi<float>() / 36,
    //                                      glm::cross(cam.getDirection(),
    //                                      cam.getTop())));
    //         return;
    //     case GLFW_KEY_LEFT:
    //         cam.setDirection(glm::rotate(cam.getDirection(),
    //                                      glm::pi<float>() / 36,
    //                                      cam.getTop()));
    //         return;
    //     case GLFW_KEY_RIGHT:
    //         cam.setDirection(glm::rotate(cam.getDirection(),
    //                                      -glm::pi<float>() / 36,
    //                                      cam.getTop()));
    //         return;
    //     default:
    //         return;
    //     }
    // });

    // unsigned char pixel[6] = "\0\0\0\0\0";
    // gl::Ctx::Get().glGetTextureSubImage(material_storage
    //                                         ->getTex(MaterialTexType_PBR::Albedo)
    //                                         ->handle(),
    //                                     0,
    //                                     0,
    //                                     0,
    //                                     0,
    //                                     1,
    //                                     1,
    //                                     1,
    //                                     GL_RGBA,
    //                                     GL_UNSIGNED_BYTE,
    //                                     4,
    //                                     pixel);

    // gl::Ctx::Get().glClearColor(0, 0, 0.2f, 1.f);
    // while (not close_window) {
    //     wnd.pollEvents();
    //     wnd.swapBuffers();

    //     gl::Ctx::Get().glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //     // mesh->draw();
    //     new_mesh->draw();
    // }

    return 0;
}