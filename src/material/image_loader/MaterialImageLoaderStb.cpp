#include "material/image_loader/MaterialImageLoaderStb.hpp"

#include <future>
#include <memory>
#include <optional>
#include <stdexcept>

#include "gl/texture/TexImage.hpp"
#include "material/base/MaterialImageLoader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

namespace gnev {

using enum MaterialImageUploadResultStb::Message;
using UploadResult = MaterialImageLoaderStb::UploadResult;
using ImageOpt = std::optional<gl::TexImage>;

MaterialImageLoaderStb::MaterialImageLoaderStb(const std::shared_ptr<
                                               base::MaterialTexStorage>& tex_storage)
    : base::MaterialImageLoader(tex_storage) {}

MaterialImageLoaderStb::~MaterialImageLoaderStb() {}

UploadResult MaterialImageLoaderStb::upload(const std::filesystem::path& path,
                                            const gl::TexImageInfo& info) {

    if (cache.contains(path)) {
        return cache[path];
    }

    std::promise<void> promise;
    cache.emplace(path, std::make_shared<MaterialImageUploadResultStb>(promise));
    auto result = cache[path];

    auto img_opt = readImage(path, info, *result);
    if (img_opt.has_value()) {
        result->index = tex_storage->initUnusedIndex();
        if (not result->index.has_value()) {
            throw std::out_of_range("");
        }
        auto tex_iter = tex_storage->operator[](result->index.value());
        tex_iter.setImage(img_opt.value());
    }

    return result;
}

ImageOpt MaterialImageLoaderStb::readImage(const std::filesystem::path& path,
                                           const gl::TexImageInfo& load_info,
                                           MaterialImageUploadResultStb& result) {
    if (not validateInfo(load_info, result)) {
        result.messages.push_back(UnsupportedInfo);
        return std::nullopt;
    }

    if (not std::filesystem::exists(path)) {
        result.messages.push_back(FileDoNotExist);
        result.messages.push_back(Failed);
        return std::nullopt;
    }

    StbInfo stb_info;
    Buffer raw_img = stbiLoad(path, stb_info, getComponents(load_info));

    gl::TexImageInfo result_info = prepareInfo(load_info, stb_info, result);
    raw_img = stbiResize(raw_img, stb_info, result_info);
    gl::TexImageData data(getBufferSize(result_info), raw_img);

    result.messages.push_back(Done);
    return gl::TexImage(result_info, data);
}

bool MaterialImageLoaderStb::validateInfo(const gl::TexImageInfo& info,
                                          MaterialImageUploadResultStb& result) {
    bool valid = true;

    if (info.x != 0) {
        result.messages.push_back(UnsupportedX);
        valid = false;
    }

    if (info.y != 0) {
        result.messages.push_back(UnsupportedY);
        valid = false;
    }

    if (info.type != GL_UNSIGNED_BYTE) {
        result.messages.push_back(UnsupportedFormat);
        valid = false;
    }

    if (getComponents(info) == 0) {
        result.messages.push_back(UnsupportedType);
        valid = false;
    }

    if (!valid) {
        result.messages.push_back(UnsupportedInfo);
    }

    return valid;
}

gl::TexImageInfo MaterialImageLoaderStb::prepareInfo(const gl::TexImageInfo& info,
                                                     const StbInfo& stb_info,
                                                     MaterialImageUploadResultStb&
                                                         result) {
    gl::TexImageInfo dst_info = info;

    if (getComponents(info) != stb_info.comp) {
        result.messages.push_back(OriginalImageHasDifferentFormat);
    }

    if (dst_info.width == 0) {
        result.messages.push_back(AutoWidth);
        dst_info.width = stb_info.width;
    }

    if (dst_info.height == 0) {
        result.messages.push_back(AutoHeight);
        dst_info.height = stb_info.height;
    }

    return dst_info;
}

unsigned int MaterialImageLoaderStb::getComponents(const gl::TexImageInfo& info) {
    switch (info.format) {
    case GL_R8:
        return 1;
    case GL_RG8:
        return 2;
    case GL_RGB8:
        return 3;
    case GL_RGBA8:
        return 4;
    default:
        return 0;
    }
}

std::size_t MaterialImageLoaderStb::getBufferSize(const gl::TexImageInfo& info) {
    return info.width * info.height * getComponents(info) * sizeof(unsigned int);
}

MaterialImageLoaderStb::Buffer MaterialImageLoaderStb::
    stbiLoad(const std::filesystem::path& path, StbInfo& stb_info, int req_comp) {
    stbi_set_flip_vertically_on_load(true);
    return {stbi_load(path.string().c_str(),
                      &stb_info.width,
                      &stb_info.height,
                      &stb_info.comp,
                      req_comp),
            &stbi_image_free};
}

MaterialImageLoaderStb::Buffer MaterialImageLoaderStb::stbiResize(const Buffer& img,
                                                                  const StbInfo& src_info,
                                                                  const gl::TexImageInfo&
                                                                      dst_info) {
    if (dst_info.width != src_info.width || dst_info.height != src_info.height) {
        Buffer resized(new GLubyte[getBufferSize(dst_info)]);
        stbir_resize_uint8(img.get(),
                           src_info.width,
                           src_info.height,
                           0,
                           resized.get(),
                           dst_info.width,
                           dst_info.height,
                           0,
                           getComponents(dst_info));
        return resized;
    }
    return img;
}

} // namespace gnev