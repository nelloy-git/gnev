#include "material/ImageLoaderStb.hpp"

#include <optional>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

namespace gnev {

ImageLoaderStb::ImageLoaderStb() {}

ImageLoaderStb::~ImageLoaderStb() {}

ImageLoaderStb::LoadResult ImageLoaderStb::load(const std::filesystem::path& path,
                                                const ImageInfo& load_info) const {
    LoadResult result{{}, std::nullopt};

    if (!validateInfo(result, load_info)) {
        result.first.push_back(LoadStatus::Failed);
        return result;
    }

    if (!std::filesystem::exists(path)) {
        result.first.push_back(LoadStatus::FileDoNotExist);
        result.first.push_back(LoadStatus::Failed);
        return result;
    }

    StbInfo stb_info;
    Buffer raw_img = stbiLoad(path, stb_info, getComponents(load_info));

    ImageInfo result_info = prepareInfo(result, load_info, stb_info);
    raw_img = stbiResize(raw_img, stb_info, result_info);
    ImageData data(getBufferSize(result_info), raw_img);

    result.first.push_back(LoadStatus::Done);
    result.second = Image(load_info, data);
    return result;
}

bool ImageLoaderStb::validateInfo(LoadResult& res, const ImageInfo& info) {
    bool valid = true;

    if (info.x != 0) {
        addMsg(res, LoadStatus::UnsupportedX);
        valid = false;
    }

    if (info.y != 0) {
        addMsg(res, LoadStatus::UnsupportedY);
        valid = false;
    }

    if (info.type != GL_UNSIGNED_BYTE) {
        addMsg(res, LoadStatus::UnsupportedFormat);
        valid = false;
    }

    if (getComponents(info) == 0) {
        addMsg(res, LoadStatus::UnsupportedType);
        valid = false;
    }

    if (!valid) {
        addMsg(res, LoadStatus::UnsupportedInfo);
    }

    return valid;
}

ImageLoaderStb::ImageInfo ImageLoaderStb::prepareInfo(LoadResult& res,
                                                      const ImageInfo& dst_info,
                                                      const StbInfo& stb_info) {
    ImageInfo info = dst_info;

    if (getComponents(info) != stb_info.comp) {
        addMsg(res, LoadStatus::OriginalImageHasDifferentFormat);
    }

    if (info.width == 0) {
        addMsg(res, LoadStatus::AutoWidth);
        info.width = stb_info.width;
    }

    if (info.height == 0) {
        addMsg(res, LoadStatus::AutoHeight);
        info.height = stb_info.height;
    }

    return info;
}

unsigned int ImageLoaderStb::getComponents(const ImageInfo& info) {
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

std::size_t ImageLoaderStb::getBufferSize(const ImageInfo& info) {
    return info.width * info.height * getComponents(info) * sizeof(unsigned int);
}

ImageLoaderStb::Buffer ImageLoaderStb::stbiLoad(const std::filesystem::path& path,
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

ImageLoaderStb::Buffer ImageLoaderStb::stbiResize(const Buffer& img,
                                                  const StbInfo& src_info,
                                                  const ImageInfo& dst_info) {
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