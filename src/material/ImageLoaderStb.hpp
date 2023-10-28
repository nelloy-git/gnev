#pragma once

#include <filesystem>
#include <memory>
#include <optional>

#include "material/base/ImageLoader.hpp"

namespace gnev {

class EXPORT ImageLoaderStb : public base::ImageLoader {
public:
    ImageLoaderStb();
    virtual ~ImageLoaderStb();

    LoadResult load(const std::filesystem::path& path,
                    const ImageInfo& info) const override;

private:
    using Buffer = std::shared_ptr<GLubyte[]>;

    struct StbInfo {
        int width;
        int height;
        int comp;
    };

    static bool validateInfo(LoadResult& res, const ImageInfo& info);
    static ImageInfo prepareInfo(LoadResult& res,
                                 const ImageInfo& info,
                                 const StbInfo& stb_info);

    static unsigned int getComponents(const ImageInfo& info);
    static std::size_t getBufferSize(const ImageInfo& info);

    static Buffer stbiLoad(const std::filesystem::path& path,
                           StbInfo& stb_info,
                           int req_comp = 0);
    static Buffer stbiResize(const Buffer& img,
                             const StbInfo& src_info,
                             const ImageInfo& dst_info);
};

} // namespace gnev