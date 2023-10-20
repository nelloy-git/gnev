#pragma once

#include <filesystem>
#include <memory>
#include <optional>

#include "gl/texture/Image.hpp"
#include "util/Util.hpp"

namespace gnev::tool {

class EXPORT StbImageLoader {
public:
    enum class LoadStatus {
        Done,
        Failed,
        FileDoNotExist,
        WrongReqComp,
        HasMoreComponents,
        HasLessComponents,
        Resized
    };

    using Image = gnev::gl::texture::Image;
    using LoadResult = std::pair<std::vector<LoadStatus>, std::optional<Image>>;

    StbImageLoader();
    virtual ~StbImageLoader();

    // "level", "z" just for output Image struct
    LoadResult load(const std::filesystem::path& path,
                    unsigned int width = 0,
                    unsigned int height = 0,
                    unsigned int req_comp = 4,
                    unsigned int level = 0,
                    unsigned int z = 0) const;

private:
    using Buffer = std::shared_ptr<GLubyte[]>;

    static Buffer stbiLoad(const std::filesystem::path& path,
                           int& img_width,
                           int& img_height,
                           int& img_comp,
                           int req_comp = 0);
    static Buffer stbiResize(const Buffer& img,
                             int img_width,
                             int img_height,
                             int dst_width,
                             int dst_height,
                             int comp);
};

} // namespace gnev::tool