#pragma once

#include <filesystem>
#include <list>
#include <unordered_map>

#include "gl/GLTextureVector.hpp"
#include "texture/TextureChunk.hpp"
#include "util/Util.hpp"

namespace gnev {

class EXPORT MaterialTextureLoader {
public:
    struct TextureLocation {
        GLuint array_index;
        GLuint pos_index;
    };

    MaterialTextureLoader(const std::shared_ptr<GladGLContext>& ctx, GLsizei mipmap_levels,
                          GLsizei img_width, GLsizei img_height, GLsizei img_components);
    virtual ~MaterialTextureLoader();

    const std::shared_ptr<GladGLContext>& ctx() const;

    TextureLocation load(const std::filesystem::path& path);
    const std::vector<GLTextureVector>& textures() const;

private:
    const GLsizei _mipmap_levels;
    const GLsizei _width;
    const GLsizei _height;
    const GLsizei _comp;
    const GLenum _storage_format;
    const GLenum _image_format;

    std::unordered_map<std::wstring, TextureLocation> _path_map;
    std::vector<GLTextureVector> _textures;

    static GLenum get_storage_format(GLsizei img_components);
    static GLenum get_image_format(GLsizei img_components);

    // GLTextureVector _color;
    // GLTextureVector _normal;
    // GLTextureVector _specular;

    // std::unordered_map<std::string, GLint> _color_path_map;
    // std::unordered_map<std::string, GLint> _normal_path_map;
    // std::unordered_map<std::string, GLint> _specular_path_map;
};

}