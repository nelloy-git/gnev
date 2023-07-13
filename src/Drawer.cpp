#include "Drawer.hpp"

#include <array>
#include <iostream>
#include <stdexcept>

#include "glm/glm.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader/ShaderBuilder.hpp"

using namespace gnev;

namespace {

const char* glGetErrorString(GLenum error){
    switch (error)
    {
    case GL_NO_ERROR:          return "No Error";
    case GL_INVALID_ENUM:      return "Invalid Enum";
    case GL_INVALID_VALUE:     return "Invalid Value";
    case GL_INVALID_OPERATION: return "Invalid Operation";
    case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid Framebuffer Operation";
    case GL_OUT_OF_MEMORY:     return "Out of Memory";
    case GL_STACK_UNDERFLOW:   return "Stack Underflow";
    case GL_STACK_OVERFLOW:    return "Stack Overflow";
    case GL_CONTEXT_LOST:      return "Context Lost";
    default:                   return "Unknown Error";
    }
};

static void glDebugOutput(GLenum source, 
                          GLenum type, 
                          unsigned int id, 
                          GLenum severity, 
                          GLsizei length, 
                          const char *message, 
                          const void *userParam){
    // auto& gl = *static_cast<const GLapi*>(userParam);
                                
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message "<< glGetErrorString(id) << "(" << id << "): " <<  message << std::endl;

    switch (source){
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type){
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity){
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
};

}


Drawer::Drawer(){
}

Drawer::~Drawer(){
}

std::pair<bool, std::string> Drawer::init(GLADloadfunc load_func){
    // Glad context
    ctx = std::make_shared<GladGLContext>(create_glad_ctx(load_func));
    if (!ctx){
        return {false, "Failed init GladContext"};
    }

    // Glfw debug
    GLint context_flags;
    ctx->GetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
    if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT){
        std::cout << "glDebugOutput enabled" << std::endl;
        ctx->Enable(GL_DEBUG_OUTPUT);
        ctx->Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        ctx->DebugMessageCallback(glDebugOutput, nullptr);
        ctx->DebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    } else {
        return {false, "Failed init glfw debug"};
    }

    // Camera
    camera = std::make_unique<Camera>(ctx);
    if (!camera){
        return {false, "Failed init camera"};
    }
    camera->pos = {1.5, 1.5,1.5};
    auto camera_mat = camera->applyLookAt({0, 0, 0});

    // Voxel VAO
    _voxel_vao = std::make_unique<GLVertexArray>(ctx);
    if (!_voxel_vao){
        return {false, "Failed init voxel VAO"};
    }
    _voxel_vao->glBindVertexArray();

    // Voxel buffer
    _voxel_buffer = std::make_unique<GLBuffer>(ctx);
    if (!_voxel_buffer){
        return {false, "Failed init voxel buffer"};
    }
    _voxel_buffer->glBindBuffer(GL_ARRAY_BUFFER);
    _voxel_buffer->glBufferStorage(_capacity * 3 * sizeof(float), nullptr, GL_DYNAMIC_STORAGE_BIT);
    ctx->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    // Texture array
    _texture_sampler = create_texture_sampler(ctx);
    _texture_chunk = create_texture_chunk(ctx);

    // Shader
    ShaderBuilder shader_builder(ctx);
    _program = shader_builder.make_program();
    if (!_program){
        return {false, shader_builder.reason};
    }
    ctx->EnableVertexAttribArray(_program->glGetAttribLocation("inPos"));
    
    ctx->Enable(GL_CULL_FACE);
    ctx->Enable(GL_DEPTH_TEST);

    // ctx->getPa

    return {true, ""};
}

void Drawer::draw() const {
    ctx->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _program->glUseProgram();
    auto index = ctx->GetUniformBlockIndex(_program->handle, "Camera");
    ctx->UniformBlockBinding(_program->handle, index, 0);
    ctx->BindBufferBase(GL_UNIFORM_BUFFER, 0, camera->buffer.handle);
    
    _voxel_vao->glBindVertexArray();

    ctx->ActiveTexture(GL_TEXTURE0);
    _texture_sampler->glBindSampler(0);
    ctx->BindTexture(GL_TEXTURE_2D_ARRAY, _texture_chunk->handle());

    ctx->DrawArrays(GL_POINTS, 0, _count);
}

void Drawer::add_voxel(float x, float y, float z){
    std::array<float, 3> data {x, y, z};
    _voxel_buffer->glBufferSubData(_count * 3 * sizeof(float), 3 * sizeof(float), data.data());
    ++_count;
}

GladGLContext Drawer::create_glad_ctx(GLADloadfunc load_func){
    GladGLContext ctx;
    gladLoadGLContext(&ctx, load_func);
    return ctx;
}

std::unique_ptr<TextureChunk> Drawer::create_texture_chunk(const std::shared_ptr<GladGLContext> &ctx){
    auto texture_chunk = std::make_unique<TextureChunk>(ctx);
    texture_chunk->init(16, 16);

    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;

    auto dirt = stbi_load("../3rdparty/minecraft_textures/dirt.png", &width, &height, &channels, 4);
    texture_chunk->add(dirt);
    stbi_image_free(dirt);
    
    auto grass_top = stbi_load("../3rdparty/minecraft_textures/dirt_path_side.png", &width, &height, &channels, 4);
    texture_chunk->add(grass_top);
    stbi_image_free(grass_top);
    
    auto grass_side = stbi_load("../3rdparty/minecraft_textures/dirt_path_top.png", &width, &height, &channels, 4);
    texture_chunk->add(grass_side);
    stbi_image_free(grass_side);

    // texture_array->glGenerateTextureMipmap();

    return texture_chunk;
}

std::unique_ptr<GLSampler> Drawer::create_texture_sampler(const std::shared_ptr<GladGLContext> &ctx){
    static const float BORDER_COLOR[] = { 1.0f, 1.0f, 1.0f, 0.0f };

    auto sampler = std::make_unique<GLSampler>(ctx);

    sampler->glSamplerParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    sampler->glSamplerParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    sampler->glSamplerParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    sampler->glSamplerParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    sampler->glSamplerParameterfv(GL_TEXTURE_BORDER_COLOR, BORDER_COLOR);

    return sampler;
}
