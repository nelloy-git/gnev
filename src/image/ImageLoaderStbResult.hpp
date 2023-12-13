#pragma once

#include "image/ImageLoader.hpp"

namespace gnev {

struct EXPORT ImageLoaderStbResult : public base::ImageLoaderResult {
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

        UnsupportedStoreX,
        UnsupportedStoreY,
        UnsupportedStoreFormat,
        UnsupportedStoreType,

        ReleasedStorage,
        FileDoNotExist,
        OriginalImageHasDifferentNumberOfComponents,
        ImageResized,
        AutoWidth,
        AutoHeight
    };

    ImageLoaderStbResult(const std::shared_future<bool>& done, Image image);
    virtual ~ImageLoaderStbResult();

    OperationStatus getStatus() const override;
    std::vector<Message> messages;

private:
    std::shared_future<bool> done;
};

EXPORT std::ostream& operator<<(std::ostream& out, const ImageLoaderStbResult& result);

} // namespace gnev

namespace nlohmann {

template <>
struct EXPORT adl_serializer<gnev::ImageLoaderStbResult::Message> {
    static void to_json(json& j, const gnev::ImageLoaderStbResult::Message& value);
};

template <>
struct EXPORT adl_serializer<gnev::ImageLoaderStbResult> {
    static void to_json(json& j, const gnev::ImageLoaderStbResult& value);
};

} // namespace nlohmann