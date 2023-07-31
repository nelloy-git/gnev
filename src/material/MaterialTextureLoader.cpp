#include "material/MaterialTextureLoader.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

using namespace gnev;

MaterialTextureLoader::MaterialTextureLoader(const std::shared_ptr<GladGLContext>& ctx, GLsizei mipmap_levels,
                                              GLsizei img_width, GLsizei img_height, GLsizei img_components)
    : _mipmap_levels(mipmap_levels),
      _width(img_width),
      _height(img_height),
      _comp(img_components),
      _storage_format(get_storage_format(img_components)),
      _image_format(get_image_format(img_components))
{
    _textures.emplace_back(ctx, _mipmap_levels, _storage_format, _width, _height);
}

MaterialTextureLoader::~MaterialTextureLoader()
{
}

const std::shared_ptr<GladGLContext>& MaterialTextureLoader::ctx() const
{
    return _textures[0].ctx();
}

MaterialTextureLoader::TextureLocation MaterialTextureLoader::load(const std::filesystem::path& path)
{
    std::cout << "Loading image " << path.string().c_str() << std::endl;
    if (!std::filesystem::exists(path)){
        throw std::runtime_error("Texture file does not exit");
    }

    auto found = _path_map.find(path.wstring());
    if (found != _path_map.end()){
        return found->second;
    }

    stbi_set_flip_vertically_on_load(true);
    int width, height, comp;
    auto img = stbi_load(path.string().c_str(), &width, &height, &comp, _comp);
    std::cout << "Width: " << width << " Height: " << height << " Comp: " << _comp << std::endl;
    if (width != _width || height != _height){
        std::cout << "Image resized" << std::endl;
        unsigned char* resized_img = static_cast<unsigned char*>(malloc(_width * _height * _comp * sizeof(unsigned char)));
        stbir_resize_uint8(img, width, height, 0, resized_img, _width, _height, 0, _comp);
        stbi_image_free(img);
        img = resized_img;
    }

    TextureLocation loc;

    if (_textures.back().size() == _textures.back().max_size()){
        _textures.emplace_back(ctx(), _mipmap_levels, _storage_format, _width, _height);
    }
    _textures.back().push_back(0, _image_format, GL_UNSIGNED_BYTE, img);
    std::cout << "Done" << std::endl;

    loc.array_index = _textures.size() - 1;
    loc.pos_index = _textures.back().size() - 1;

    _path_map[path.wstring()] = loc;
    return loc;
}

const std::vector<GLTextureVector>& MaterialTextureLoader::textures() const
{
    return _textures;
}

GLenum MaterialTextureLoader::get_storage_format(GLsizei img_components)
{
    switch (img_components)
    {
    case 1: return GL_R8;
    case 2: return GL_RG8;
    case 3: return GL_RGB8;
    case 4: return GL_RGBA8;
    
    default:
        throw std::out_of_range("Unsupported number of image components");
    }
}

GLenum MaterialTextureLoader::get_image_format(GLsizei img_components)
{
    switch (img_components)
    {
    case 1: return GL_RED;
    case 2: return GL_RG;
    case 3: return GL_RGB;
    case 4: return GL_RGBA;
    
    default:
        throw std::out_of_range("Unsupported number of image components");
    }
}