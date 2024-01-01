#include "image/ImageLoaderStb.hpp"

#include <iostream>

#include "image/ImageLoaderStbResult.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

namespace gnev {

using Result = ImageLoaderStbResult;
using enum Result::Message;

Ref<base::ImageLoaderResult> ImageLoaderStb::load(const std::filesystem::path& path,
                                                  const ImageInfo& read_info,
                                                  const ImageInfo& store_info) {
    std::promise<bool> done;
    auto result =
        MakeSharable<ImageLoaderStbResult>(done.get_future(), Image{read_info, 1});

    if (not validateReadInfo(read_info, result)) {
        done.set_value(false);
        return result;
    }

    if (not validateStoreInfo(store_info, result)) {
        done.set_value(false);
        return result;
    }

    if (read_info.format != store_info.format or read_info.type != store_info.type) {
        done.set_value(false);
        return result;
    }

    auto img_opt = stbLoad(path, read_info, result);
    if (not img_opt.has_value()) {
        done.set_value(false);
        return result;
    }

    Image img = stbResize(img_opt.value(), store_info, result);

    std::cout << "STB_3 [0,0]: ";
    for (int i = 0; i < 4; i++) {
        std::cout << int(img_opt.value().data.buffer[i]) << " ";
    }
    std::cout << std::endl;

    result->image = img;
    result->messages.push_back(Done);
    done.set_value(true);
    return result;
}

bool ImageLoaderStb::validateReadInfo(const ImageInfo& read_info,
                                      ImageLoaderStbResult& result) const {
    bool is_valid = true;

    if (read_info.level != 0) {
        result.messages.push_back(UnsupportedReadLevel);
        is_valid = false;
    }

    if (read_info.x != 0) {
        result.messages.push_back(UnsupportedReadX);
        is_valid = false;
    }

    if (read_info.y != 0) {
        result.messages.push_back(UnsupportedReadY);
        is_valid = false;
    }

    if (getComponents(read_info) == 0) {
        result.messages.push_back(UnsupportedReadFormat);
        is_valid = false;
    }

    if (read_info.type != GL_UNSIGNED_BYTE) {
        result.messages.push_back(UnsupportedReadType);
        is_valid = false;
    }

    return is_valid;
}

bool ImageLoaderStb::validateStoreInfo(const ImageInfo& store_info,
                                       ImageLoaderStbResult& result) const {
    bool is_valid = true;

    if (store_info.x != 0) {
        result.messages.push_back(UnsupportedStoreX);
        is_valid = false;
    }

    if (store_info.y != 0) {
        result.messages.push_back(UnsupportedStoreY);
        is_valid = false;
    }

    if (getComponents(store_info) == 0) {
        result.messages.push_back(UnsupportedStoreFormat);
        is_valid = false;
    }

    if (store_info.type != GL_UNSIGNED_BYTE) {
        result.messages.push_back(UnsupportedStoreType);
        is_valid = false;
    }

    return is_valid;
}

unsigned int ImageLoaderStb::getComponents(const ImageInfo& info) const {
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

std::size_t ImageLoaderStb::getBufferSize(const ImageInfo& info) const {
    return info.width * info.height * getComponents(info) * sizeof(unsigned int);
}

std::optional<Image> ImageLoaderStb::stbLoad(const std::filesystem::path& path,
                                             const ImageInfo& read_info,
                                             ImageLoaderStbResult& result) const {
    stbi_set_flip_vertically_on_load(true);

    int w, h, c;
    std::shared_ptr<GLubyte[]> ptr{
        stbi_load(path.string().c_str(), &w, &h, &c, getComponents(read_info)),
        &stbi_image_free};

    auto received_info = read_info;
    if (received_info.width == 0) {
        received_info.width = w;
    }

    if (received_info.height == 0) {
        received_info.height = h;
    }

    if (received_info.width != w) {
        result.messages.push_back(UnsupportedReadWidth);
        return std::nullopt;
    }

    if (received_info.height != h) {
        result.messages.push_back(UnsupportedReadHeight);
        return std::nullopt;
    }

    return Image{read_info, {getBufferSize(read_info), ptr}};
}

Image ImageLoaderStb::stbResize(const Image& image,
                                const ImageInfo& store_info,
                                ImageLoaderStbResult& result) const {
    std::cout << "Resizing: " << image.info.width << "x" << image.info.height << " to "
              << store_info.width << "x" << store_info.height << std::endl;

    if (image.info.width == store_info.width or image.info.height == store_info.height) {
        return image;
    }

    Image resized{store_info, getBufferSize(store_info)};
    stbir_resize(image.data.get<GLubyte>(),
                 image.info.width,
                 image.info.height,
                 0,
                 resized.data.get<GLubyte>(),
                 resized.info.width,
                 resized.info.height,
                 0,
                 stbir_pixel_layout::STBIR_RGBA,
                 stbir_datatype::STBIR_TYPE_UINT8,
                 stbir_edge::STBIR_EDGE_CLAMP,
                 stbir_filter::STBIR_FILTER_DEFAULT);
    return resized;
}

} // namespace gnev