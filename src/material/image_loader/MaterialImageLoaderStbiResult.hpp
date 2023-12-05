#pragma once

#include <future>
#include <iostream>
#include <vector>

#include "nlohmann/json_fwd.hpp"
#include "util/Export.hpp"
#include "util/OperationResult.hpp"

namespace gnev {

struct MaterialImageLoaderStbiResult : public OperationResult {
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

    MaterialImageLoaderStbiResult(std::future<bool>&& done);
    virtual ~MaterialImageLoaderStbiResult();

    OperationStatus getStatus() const override;
    std::vector<Message> messages;

private:
    std::shared_future<bool> done;
};

EXPORT std::ostream&
operator<<(std::ostream& out, const MaterialImageLoaderStbiResult& result);

} // namespace gnev

namespace nlohmann {

template <>
struct EXPORT adl_serializer<gnev::MaterialImageLoaderStbiResult::Message> {
    static void to_json(json& j, const gnev::MaterialImageLoaderStbiResult::Message& value);
};

template <>
struct EXPORT adl_serializer<gnev::MaterialImageLoaderStbiResult> {
    static void to_json(json& j, const gnev::MaterialImageLoaderStbiResult& value);
};

} // namespace nlohmann