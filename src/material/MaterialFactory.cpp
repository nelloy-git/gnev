#include "material/MaterialFactory.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <thread>

#include "nlohmann/json.hpp"

using namespace gnev;
using json = nlohmann::json;

MaterialFactory::MaterialFactory(const std::shared_ptr<GladGLContext>& ctx, GLsizei tex_mipmap_levels, GLsizei tex_width, GLsizei tex_height)
    : _texture_loader(ctx, tex_mipmap_levels, tex_width, tex_height, 4),
      _buffer(ctx, nullptr, 0, GL_STATIC_DRAW)
{
}

MaterialFactory::~MaterialFactory(){   
}

const MaterialTextureLoader& MaterialFactory::texture_loader() const
{
    return _texture_loader;
}

GLint MaterialFactory::register_material(const std::wstring& name, const MaterialInfo& info)
{
    auto found = _id_map.find(name);
    if (found != _id_map.end()){
        std::string str;
        size_t size;
        str.resize(name.length());
        wcstombs_s(&size, &str[0], str.size() + 1, name.c_str(), name.size());
        throw std::runtime_error("Material " + str + " already exists");
    }

    Material material;
    _load_color_texture(material, info);

}

void MaterialFactory::_load_color_texture(Material& material, const MaterialInfo& info)
{
    if (info.texture.color_path.has_value()){
        auto& path = info.texture.color_path.value();
        GLint array_index = -1;
        GLint pos_index = -1;
        try {
            auto loc = _texture_loader.load(path);
            array_index = loc.array_index;
            pos_index = loc.pos_index;
        } catch (std::exception& e){
            std::cout << "can not load texture" << std::endl;
        }
        material.texture.color_binding_index = array_index;
        material.texture.color_element_index = pos_index;
    }
 
}