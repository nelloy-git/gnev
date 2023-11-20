#include "material/image_loader/MaterialImageLoaderStb.hpp"

#include <exception>
#include <future>
#include <memory>
#include <optional>
#include <stdexcept>

#include "gl/texture/TexImage.hpp"
#include "material/base/MaterialImageLoader.hpp"
#include "material/base/MaterialTexStorage.hpp"
#include "material/pbr/MaterialGL_PBR.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

namespace gnev {

using enum MaterialImageLoaderStbResult::Message;
using Result = MaterialImageLoaderStbResult;
using ImageOpt = std::optional<gl::TexImage>;

MaterialImageLoaderStb::MaterialImageLoaderStb() {}

MaterialImageLoaderStb::~MaterialImageLoaderStb() {}

std::shared_ptr<base::MaterialImageLoaderResult>
MaterialImageLoaderStb::upload(std::weak_ptr<base::MaterialTexStorage> weak_tex_storage,
                               const std::filesystem::path& path,
                               const gl::TexImageInfo& info) {

    if (cache.contains(path)) {
        return cache[path];
    }

    auto p_tex_storage = weak_tex_storage.lock();
    if (not p_tex_storage) {
        throw std::runtime_error("");
    }

    std::promise<bool> done;
    base::MaterialTexRef tex_ref(weak_tex_storage);
    auto result =
        std::make_shared<MaterialImageLoaderStbResult>(done.get_future(), tex_ref);
    cache.emplace(path, result);

    auto& storage = *p_tex_storage;
    std::optional<gl::TexImage> img_opt;
    try {
        img_opt = readImage(path, info, *result);
        if (not img_opt.has_value()) {
            throw std::runtime_error("");
        }
        storage.at(*tex_ref.getIndex()).setImage(img_opt.value());
    } catch (...) {
        done.set_exception(std::current_exception());
    }

    done.set_value(true);
    return result;
}

ImageOpt MaterialImageLoaderStb::readImage(const std::filesystem::path& path,
                                           const gl::TexImageInfo& load_info,
                                           MaterialImageLoaderStbResult& result) {
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
    raw_img = stbiResize(raw_img, stb_info, result_info, result);
    gl::TexImageData data(getBufferSize(result_info), raw_img);

    result.messages.push_back(Done);
    return gl::TexImage(result_info, data);
}

bool MaterialImageLoaderStb::validateInfo(const gl::TexImageInfo& info,
                                          MaterialImageLoaderStbResult& result) {
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

gl::TexImageInfo
MaterialImageLoaderStb::prepareInfo(const gl::TexImageInfo& info,
                                    const StbInfo& stb_info,
                                    MaterialImageLoaderStbResult& result) {
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
    case GL_RED:
        return 1;
    case GL_RG:
        return 2;
    case GL_RGB:
        return 3;
    case GL_RGBA:
        return 4;
    default:
        return 0;
    }
}

std::size_t MaterialImageLoaderStb::getBufferSize(const gl::TexImageInfo& info) {
    return info.width * info.height * getComponents(info) * sizeof(unsigned int);
}

MaterialImageLoaderStb::Buffer
MaterialImageLoaderStb::stbiLoad(const std::filesystem::path& path,
                                 StbInfo& stb_info,
                                 int req_comp) {
    stbi_set_flip_vertically_on_load(true);
    return {stbi_load(path.string().c_str(),
                      &stb_info.width,
                      &stb_info.height,
                      &stb_info.comp,
                      req_comp),
            &stbi_image_free};
}

MaterialImageLoaderStb::Buffer
MaterialImageLoaderStb::stbiResize(const Buffer& img,
                                   const StbInfo& src_info,
                                   const gl::TexImageInfo& dst_info,
                                   MaterialImageLoaderStbResult& result) {
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
        result.messages.push_back(ImageResized);
        return resized;
    }
    return img;
}

} // namespace gnev