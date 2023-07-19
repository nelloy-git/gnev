#include "texture/TextureController.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace gnev;

TextureController::TextureController(const std::shared_ptr<GladGLContext>& ctx, GLsizei img_width, GLsizei img_height){
    _chunks.push_back(TextureChunk(ctx, img_width, img_height));
}

TextureController::~TextureController(){
}

GLuint TextureController::bind(GLenum first_target) const {
    for (int i = 0; i < _chunks.size(); ++i){
        ctx()->ActiveTexture(first_target + i);
        _chunks[i].texture().glBindTexture(GL_TEXTURE_2D_ARRAY);
    }
    return _chunks.size();
}

GLint TextureController::load(const std::filesystem::path& path){
    auto found = _path_map.find(path.string());
    if (found != _path_map.end()){
        return found->second;
    }

    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;

    if (!std::filesystem::exists(path)){
        std::cout << "Do not exist" << std:: endl;
        return -1;
    }

    auto img = stbi_load(path.string().c_str(), &width, &height, &channels, 4);
    auto id = _chunks.back().add(img);

    if (id < 0){
        auto img_w = _chunks.back().width();
        auto img_h = _chunks.back().height();

        _chunks.push_back(TextureChunk(ctx(), width, height));
        id = _chunks.back().add(img);
    }
    stbi_image_free(img);

    _path_map[path.string()] = id;
    return id;
}

const std::shared_ptr<GladGLContext>& TextureController::ctx() const {
    return _chunks[0].ctx();
}