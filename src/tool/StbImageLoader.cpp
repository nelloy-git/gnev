#include "tool/StbImageLoader.hpp"

#include <optional>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

namespace gnev::tool {

StbImageLoader::~StbImageLoader() {}

StbImageLoader::LoadResult StbImageLoader::load(const std::filesystem::path& path,
                                                unsigned int width,
                                                unsigned int height,
                                                unsigned int req_comp) const {
    LoadResult result{{}, std::nullopt};

    if (!std::filesystem::exists(path)) {
        result.first.push_back(LoadStatus::FileDoNotExist);
        result.first.push_back(LoadStatus::Failed);
        return result;
    }

    int img_width, img_height, img_comp;
    auto raw_img = stbiLoad(path, img_width, img_height, img_comp, req_comp);

    if (width != img_width || height != img_height) {
        result.first.push_back(LoadStatus::Resized);
        raw_img = stbiResize(raw_img, img_width, img_height, width, height, req_comp);
    }

    req_comp = req_comp == 0 ? img_comp : req_comp;
    GLenum format;
    switch (req_comp) {
    case 1:
        format = GL_R8;
        break;
    case 2:
        format = GL_RG8;
        break;
    case 3:
        format = GL_RGB8;
        break;
    case 4:
        format = GL_RGBA8;
        break;
    default:
        result.first.push_back(LoadStatus::WrongReqComp);
        result.first.push_back(LoadStatus::Failed);
        return result;
    }

    gl::texture::ImageInfo info = {.level = 0,
                                   .x = 0,
                                   .y = 0,
                                   .z = 0,
                                   .width = width,
                                   .height = height,
                                   .format = format,
                                   .type = GL_UNSIGNED_BYTE};
    gl::texture::ImageData data(width * height * req_comp * sizeof(GLubyte), raw_img);

    result.first.push_back(LoadStatus::Done);
    result.second = gl::texture::Image(info, data);
    return result;
}

StbImageLoader::Buffer StbImageLoader::stbiLoad(const std::filesystem::path& path,
                                                int& img_width,
                                                int& img_height,
                                                int& img_comp,
                                                int req_comp) {
    return {
        stbi_load(path.string().c_str(), &img_width, &img_height, &img_comp, req_comp),
        &stbi_image_free};
}

StbImageLoader::Buffer StbImageLoader::stbiResize(const Buffer& img,
                                                  int img_width,
                                                  int img_height,
                                                  int dst_width,
                                                  int dst_height,
                                                  int comp) {
    Buffer resized(new GLubyte[dst_width * dst_height * comp * sizeof(GLubyte)]);
    stbir_resize_uint8(img.get(),
                       img_width,
                       img_height,
                       0,
                       resized.get(),
                       dst_width,
                       dst_height,
                       0,
                       comp);
    return resized;
}

} // namespace gnev::tool