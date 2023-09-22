#pragma once

#include <filesystem>
#include <list>
#include <unordered_map>
#include <vector>

#include "gl/TextureVector.hpp"
#include "util/Util.hpp"

namespace gnev {

class EXPORT MaterialTextureLoader {
public:
    struct TextureLocation {
        GLuint array_index;
        GLuint pos_index;
    };

    MaterialTextureLoader(const std::shared_ptr<GladGLContext>& ctx,
                          GLsizei mipmap_levels,
                          GLsizei img_width,
                          GLsizei img_height,
                          GLsizei img_components);
    virtual ~MaterialTextureLoader();

    const std::shared_ptr<GladGLContext>& ctx() const;

    TextureLocation load(const std::filesystem::path& path);
    const std::vector<gl::TextureVector>& textures() const;

private:
    const GLsizei _mipmap_levels;
    const GLsizei _width;
    const GLsizei _height;
    const GLsizei _comp;
    const GLenum _storage_format;
    const GLenum _image_format;

    std::unordered_map<std::wstring, TextureLocation> _path_map;
    std::vector<gl::TextureVector> _textures;

    static GLenum get_storage_format(GLsizei img_components);
    static GLenum get_image_format(GLsizei img_components);
};

} // namespace gnev