#include "image/ImageLoader.hpp"

#include "util/InstanceLogger.hpp"

namespace gnev::base {

std::unique_ptr<InstanceLogger>
ImageLoader::Logger(const SrcLoc& src_loc) const {
    return std::make_unique<InstanceLogger>(src_loc);
}

} // namespace gnev::base