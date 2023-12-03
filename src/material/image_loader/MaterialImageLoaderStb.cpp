#include "material/image_loader/MaterialImageLoaderStb.hpp"

#include <exception>
#include <future>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>

#include "gl/texture/TexImage.hpp"
#include "material/base/MaterialImageLoader.hpp"
#include "material/pbr/MaterialGL_PBR.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

namespace gnev {

using enum MaterialImageLoaderStbiResult::Message;
using Result = MaterialImageLoaderStbiResult;
using ImageOpt = std::optional<gl::TexImage>;

MaterialImageLoaderStbi::MaterialImageLoaderStbi() {}

MaterialImageLoaderStbi::~MaterialImageLoaderStbi() {}

Ref<base::MaterialImageLoaderResult>
MaterialImageLoaderStbi::upload(Ref<base::MaterialTex> tex_ref,
                                const std::filesystem::path& path,
                                const gl::TexImageInfo& read_info,
                                const gl::TexImageInfo& write_info) {
    if (cache.contains(path)) {
        using enum std::future_status;

        Ref<MaterialImageLoaderStbiResult>& result(cache.at(path));
        auto status = result->done.wait_for(std::chrono::seconds(0));
        if (status == timeout or (status == ready and result->done.get())) {
            return result;
        }
    }

    std::promise<bool> done;
    auto result = MakeSharable<MaterialImageLoaderStbiResult>(done.get_future(), tex_ref);
    cache.emplace(path, result);

    auto storage_opt = tex_ref->getWeakStorage().lock();
    if (not storage_opt.has_value()) {
        result->messages.push_back(ReleasedStorage);
        result->messages.push_back(Failed);
        done.set_value(false);
        return result;
    }
    auto& storage = storage_opt.value();

    if (not validateInfos(read_info, write_info, result)) {
        result->messages.push_back(Failed);
        done.set_value(false);
        return result;
    }

    std::optional<gl::TexImage> img_opt;
    try {
        img_opt = readImage(path, read_info, write_info, result);
        if (not img_opt.has_value()) {
            result->messages.push_back(Failed);
            done.set_value(false);
            return result;
        }
        storage->setData(tex_ref->getIndex(), img_opt.value());
    } catch (...) {
        done.set_exception(std::current_exception());
    }

    done.set_value(true);
    return result;
}

ImageOpt MaterialImageLoaderStbi::readImage(const std::filesystem::path& path,
                                            const gl::TexImageInfo& read_info,
                                            const gl::TexImageInfo& write_info,
                                            MaterialImageLoaderStbiResult& result) {
    if (not std::filesystem::exists(path)) {
        result.messages.push_back(FileDoNotExist);
        return std::nullopt;
    }

    StbInfo stb_info;
    Buffer raw_img = stbiLoad(path, stb_info, getComponents(read_info));
    if (getComponents(read_info) != stb_info.comp) {
        result.messages.push_back(OriginalImageHasDifferentNumberOfComponents);
    }

    raw_img = stbiResize(raw_img, stb_info, write_info, result);
    gl::TexImageData data(getBufferSize(write_info), raw_img);

    result.messages.push_back(Done);
    return gl::TexImage(write_info, data);
}

bool MaterialImageLoaderStbi::validateInfos(const gl::TexImageInfo& read_info,
                                            const gl::TexImageInfo& write_info,
                                            MaterialImageLoaderStbiResult& result) {
    bool valid_read = true;
    bool valid_write = true;

    if (read_info.level != 0) {
        result.messages.push_back(UnsupportedReadLevel);
        valid_read = false;
    }

    if (read_info.x != 0) {
        result.messages.push_back(UnsupportedReadX);
        valid_read = false;
    }

    if (read_info.y != 0) {
        result.messages.push_back(UnsupportedReadY);
        valid_read = false;
    }

    if (read_info.type != GL_UNSIGNED_BYTE) {
        result.messages.push_back(UnsupportedReadType);
        valid_read = false;
    }

    if (getComponents(read_info) == 0) {
        result.messages.push_back(UnsupportedReadFormat);
        valid_read = false;
    }

    if (!valid_read) {
        result.messages.push_back(UnsupportedReadInfo);
    }

    if (write_info.x != 0) {
        result.messages.push_back(UnsupportedWriteX);
        valid_write = false;
    }

    if (write_info.y != 0) {
        result.messages.push_back(UnsupportedWriteY);
        valid_write = false;
    }

    if (write_info.type != GL_UNSIGNED_BYTE) {
        result.messages.push_back(UnsupportedWriteType);
        valid_write = false;
    }

    if (getComponents(write_info) != getComponents(read_info)) {
        result.messages.push_back(UnsupportedWriteFormat);
        valid_write = false;
    }

    if (!valid_write) {
        result.messages.push_back(UnsupportedWriteInfo);
    }

    return valid_read and valid_write;
}

unsigned int MaterialImageLoaderStbi::getComponents(const gl::TexImageInfo& info) {
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

std::size_t MaterialImageLoaderStbi::getBufferSize(const gl::TexImageInfo& info) {
    return info.width * info.height * getComponents(info) * sizeof(unsigned int);
}

MaterialImageLoaderStbi::Buffer
MaterialImageLoaderStbi::stbiLoad(const std::filesystem::path& path,
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

MaterialImageLoaderStbi::Buffer
MaterialImageLoaderStbi::stbiResize(const Buffer& img,
                                    const StbInfo& src_info,
                                    const gl::TexImageInfo& dst_info,
                                    MaterialImageLoaderStbiResult& result) {
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