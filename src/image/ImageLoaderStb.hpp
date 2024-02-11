#pragma once

#include "image/ImageLoader.hpp"
#include "image/ImageLoaderStbResult.hpp"

namespace gnev {

class EXPORT ImageLoaderStb : public base::ImageLoader {
public:
    Ref<base::ImageLoaderResult> load(const std::filesystem::path& path,
                                      const ImageInfo& read_info,
                                      const ImageInfo& store_info) override;

private:
    bool validateReadInfo(const ImageInfo& read_info, ImageLoaderStbResult& result) const;
    bool validateStoreInfo(const ImageInfo& store_info,
                           ImageLoaderStbResult& result) const;
    unsigned int getComponents(const ImageInfo& info) const;
    std::size_t getBufferSize(const ImageInfo& info) const;

    std::optional<Image> stbLoad(const std::filesystem::path& path,
                                 const ImageInfo& read_info,
                                 ImageLoaderStbResult& result) const;

    Image stbResize(const Image& image,
                    const ImageInfo& store_info,
                    ImageLoaderStbResult& result) const;

    
};

} // namespace gnev