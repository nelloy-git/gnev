#pragma once

#include <filesystem>

#include "gl/texture/Image.hpp"

namespace gnev::base {

class EXPORT ImageLoader {
public:
    using Image = gl::texture::Image;
    using ImageData = gl::texture::ImageData;
    using ImageInfo = gl::texture::ImageInfo;

    enum class LoadStatus {
        Done,
        Failed,
        UnsupportedLevel,
        UnsupportedX,
        UnsupportedY,
        UnsupportedWidth,
        UnsupportedHeight,
        UnsupportedFormat,
        UnsupportedType,
        UnsupportedInfo,
        FileDoNotExist,
        OriginalImageHasDifferentFormat,
        ImageResized,
        AutoFormat,
        AutoWidth,
        AutoHeight
    };

    ImageLoader() {}

    virtual ~ImageLoader(){};

    using LoadResult = std::pair<std::vector<LoadStatus>, std::optional<Image>>;

    virtual LoadResult load(const std::filesystem::path& path,
                            const ImageInfo& info) const = 0;

protected:
    static void addMsg(LoadResult& res, LoadStatus status) { res.first.push_back(status); }
};

} // namespace gnev::base