#pragma once

#include <filesystem>
#include <memory>
#include <optional>

#include "gl/GLBufferVectorT.hpp"

#include "material/MaterialTextureLoader.hpp"

namespace gnev {

struct MaterialInfo {
    GLfloat color[4] = {1, 1, 1, 1};
    GLfloat normal[3] = {0, 0, 1};
    GLfloat specular[3] = {0, 0, 0};
    GLfloat ambient[3];
    GLfloat diffuse[3];
    struct {
        std::optional<std::filesystem::path> color_path = std::nullopt;
        std::optional<std::filesystem::path> normal_path = std::nullopt;
        std::optional<std::filesystem::path> specular_path = std::nullopt;
    } texture;
};

struct Material {
    GLfloat color[4];
    GLfloat normal[3];
    GLfloat specular[3];
    GLfloat ambient[3];
    GLfloat diffuse[3];
    struct {
        GLint color_binding_index;
        GLint color_element_index;
        GLint normal_binding_index;
        GLint normal_element_index;
        GLint specular_binding_index;
        GLint specular_element_index;
    } texture;
};

class EXPORT MaterialFactory {
public:
    MaterialFactory(const std::shared_ptr<GladGLContext>& ctx, GLsizei tex_mipmap_levels, GLsizei tex_width, GLsizei tex_height);
    virtual ~MaterialFactory();

    const MaterialTextureLoader& texture_loader() const;

    GLint register_material(const std::wstring& name, const MaterialInfo& info);


    GLuint register_json(const std::filesystem::path& path);


private:
    MaterialTextureLoader _texture_loader;

    std::unordered_map<std::wstring, GLint> _id_map;
    GLBufferVectorT<Material> _buffer;

    void _load_color_texture(Material& material, const MaterialInfo& info);
    void _load_color_texture(Material& material, const MaterialInfo& info);
    void _load_color_texture(Material& material, const MaterialInfo& info);
};

}