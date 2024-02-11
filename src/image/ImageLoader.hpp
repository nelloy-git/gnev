#pragma once

#include <filesystem>

#include "image/Image.hpp"
#include "util/OperationResult.hpp"
#include "util/Ref.hpp"

namespace gnev {
class InstanceLogger;
};

namespace gnev::base {

class ImageLoaderResult : public OperationResult {
public:
    ImageLoaderResult(Image image)
        : image(image) {}

    Image image;
};

class ImageLoader {
public:
    virtual Ref<ImageLoaderResult> load(const std::filesystem::path& path,
                                        const ImageInfo& read_info,
                                        const ImageInfo& store_info) = 0;

    std::unique_ptr<InstanceLogger>
    Logger(const SrcLoc& src_loc = SrcLoc::Current()) const;
};

} // namespace gnev::base