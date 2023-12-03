#pragma once

#include "material/base/MaterialImageLoader.hpp"

namespace gnev {

struct MaterialImageLoaderStbiResult : public base::MaterialImageLoaderResult {
    enum class Message {
        Done,
        Failed,

        UnsupportedReadLevel,
        UnsupportedReadX,
        UnsupportedReadY,
        UnsupportedReadWidth,
        UnsupportedReadHeight,
        UnsupportedReadFormat,
        UnsupportedReadType,
        UnsupportedReadInfo,

        UnsupportedWriteLevel,
        UnsupportedWriteX,
        UnsupportedWriteY,
        UnsupportedWriteWidth,
        UnsupportedWriteHeight,
        UnsupportedWriteFormat,
        UnsupportedWriteType,
        UnsupportedWriteInfo,

        ReleasedStorage,
        FileDoNotExist,
        OriginalImageHasDifferentNumberOfComponents,
        ImageResized,
        AutoWidth,
        AutoHeight
    };

    MaterialImageLoaderStbiResult(std::shared_future<bool>&& done,
                                  WeakRef<base::MaterialTex> tex_ref);

    ~MaterialImageLoaderStbiResult();

    std::vector<Message> messages;
};

std::ostream& operator<<(std::ostream& out, const MaterialImageLoaderStbiResult& result);

} // namespace gnev