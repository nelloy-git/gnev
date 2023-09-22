#include "material/MaterialFactory.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace gnev;

MaterialFactory::MaterialFactory(const std::shared_ptr<GladGLContext>& ctx,
                                 GLsizei diffuse_mipmap_levels,
                                 GLsizei diffuse_width,
                                 GLsizei diffuse_height,
                                 GLsizei normal_mipmap_levels,
                                 GLsizei normal_width,
                                 GLsizei normal_height,
                                 GLsizei specular_mipmap_levels,
                                 GLsizei specular_width,
                                 GLsizei specular_height)
    : _diffuse_loader(ctx, diffuse_mipmap_levels, diffuse_width, diffuse_height, 4)
    , _normal_loader(ctx, normal_mipmap_levels, normal_width, normal_height, 4)
    , _specular_loader(ctx, normal_mipmap_levels, normal_width, normal_height, 4)
    , _buffer(ctx, 0, nullptr) {}

MaterialFactory::~MaterialFactory() {}

const gl::BufferVector<Material>& MaterialFactory::material_buffer() const { return _buffer; }

const MaterialTextureLoader& MaterialFactory::diffuse_loader() const { return _diffuse_loader; }

const MaterialTextureLoader& MaterialFactory::normal_loader() const { return _normal_loader; }

const MaterialTextureLoader& MaterialFactory::specular_loader() const { return _specular_loader; }

GLint MaterialFactory::register_material(const std::wstring& name, const MaterialInfo& info) {
    auto found = _id_map.find(name);
    if (found != _id_map.end()) {
        std::string str;
        size_t size;
        str.resize(name.length());
        // wcstombs_s(&size, &str[0], str.size() + 1, name.c_str(), name.size());
        throw std::runtime_error("Material " + str + " already exists");
    }

    Material material;
    std::copy(info.color, info.color + std::size(material.color), material.color);
    std::copy(info.normal, info.normal + std::size(material.normal), material.normal);
    std::copy(info.specular, info.specular + std::size(material.specular), material.specular);

    _load_diffuse_texture(material, info);
    _load_normal_texture(material, info);
    _load_specular_texture(material, info);

    _buffer.push_back(material);
    return _buffer.size() - 1;
}

void MaterialFactory::_load_diffuse_texture(Material& material, const MaterialInfo& info) {
    if (info.texture.diffuse_path.has_value()) {
        auto& path = info.texture.diffuse_path.value();
        GLint array_index = -1;
        GLint pos_index = -1;
        try {
            auto loc = _diffuse_loader.load(path);
            array_index = loc.array_index;
            pos_index = loc.pos_index;
        } catch (std::exception& e) {
            std::cout << "can not load texture" << std::endl;
        }
        material.texture.diffuse_array_index = array_index;
        material.texture.diffuse_element_index = pos_index;
    }
}

void MaterialFactory::_load_normal_texture(Material& material, const MaterialInfo& info) {
    if (info.texture.normal_path.has_value()) {
        auto& path = info.texture.normal_path.value();
        GLint array_index = -1;
        GLint pos_index = -1;
        try {
            auto loc = _normal_loader.load(path);
            array_index = loc.array_index;
            pos_index = loc.pos_index;
        } catch (std::exception& e) {
            std::cout << "can not load texture" << std::endl;
        }
        material.texture.normal_array_index = array_index;
        material.texture.normal_element_index = pos_index;
    }
}

void MaterialFactory::_load_specular_texture(Material& material, const MaterialInfo& info) {
    if (info.texture.specular_path.has_value()) {
        auto& path = info.texture.specular_path.value();
        GLint array_index = -1;
        GLint pos_index = -1;
        try {
            auto loc = _specular_loader.load(path);
            array_index = loc.array_index;
            pos_index = loc.pos_index;
        } catch (std::exception& e) {
            std::cout << "can not load texture" << std::endl;
        }
        material.texture.specular_array_index = array_index;
        material.texture.specular_element_index = pos_index;
    }
}