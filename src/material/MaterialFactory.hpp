#pragma once

#include <filesystem>
#include <memory>
#include <optional>

#include "gl/BufferVector.hpp"

#include "material/MaterialTextureLoader.hpp"

namespace gnev {

struct MaterialInfo {
    GLfloat color[4] = {1, 1, 1, 1};
    GLfloat normal[4] = {0, 0, 1, 0};
    GLfloat specular[4] = {0, 0, 0, 0};
    struct {
        std::optional<std::filesystem::path> diffuse_path = std::nullopt;
        std::optional<std::filesystem::path> normal_path = std::nullopt;
        std::optional<std::filesystem::path> specular_path = std::nullopt;
    } texture;
};

struct Material {
    GLfloat color[4];
    GLfloat normal[4];
    GLfloat specular[4];
    struct {
        GLint diffuse_array_index;
        GLint diffuse_element_index;
        GLint normal_array_index;
        GLint normal_element_index;
        GLint specular_array_index;
        GLint specular_element_index;
    } texture;
};

class EXPORT MaterialFactory {
public:
    MaterialFactory(const std::shared_ptr<GladGLContext>& ctx,
                    GLsizei diffuse_mipmap_levels, GLsizei diffuse_width, GLsizei diffuse_height,
                    GLsizei normal_mipmap_levels, GLsizei normal_width, GLsizei normal_height,
                    GLsizei specular_mipmap_levels, GLsizei specular_width, GLsizei specular_height);
    virtual ~MaterialFactory();

    const gl::BufferVector<Material>& material_buffer() const;
    const MaterialTextureLoader& diffuse_loader() const;
    const MaterialTextureLoader& normal_loader() const;
    const MaterialTextureLoader& specular_loader() const;

    GLint register_material(const std::wstring& name, const MaterialInfo& info);


private:
    MaterialTextureLoader _diffuse_loader;
    MaterialTextureLoader _normal_loader;
    MaterialTextureLoader _specular_loader;

    std::unordered_map<std::wstring, GLint> _id_map;
    gl::BufferVector<Material> _buffer;

    void _load_diffuse_texture(Material& material, const MaterialInfo& info);
    void _load_normal_texture(Material& material, const MaterialInfo& info);
    void _load_specular_texture(Material& material, const MaterialInfo& info);
};

}