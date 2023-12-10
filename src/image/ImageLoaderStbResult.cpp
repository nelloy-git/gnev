#include "image/ImageLoaderStbResult.hpp"

#include "magic_enum.hpp"
#include "nlohmann/json.hpp"

namespace gnev {

ImageLoaderStbResult::ImageLoaderStbResult(const std::shared_future<bool>& done,
                                           Image image)
    : base::ImageLoaderResult(image)
    , done(done) {}

ImageLoaderStbResult::~ImageLoaderStbResult(){};

OperationStatus ImageLoaderStbResult::getStatus() const {
    auto s = done.wait_for(std::chrono::seconds(0));
    if (s == std::future_status::timeout) {
        return OperationStatus::InProgress;
    }

    try {
        if (done.get()) {
            return OperationStatus::Done;
        } else {
            return OperationStatus::Failed;
        }
    } catch (...) {
        return OperationStatus::Failed;
    }
}

std::ostream& operator<<(std::ostream& out, const ImageLoaderStbResult& value) {
    out << nlohmann::json(value);
    return out;
}

} // namespace gnev

namespace nlohmann {

void adl_serializer<gnev::ImageLoaderStbResult::Message>::
    to_json(json& j, const gnev::ImageLoaderStbResult::Message& value) {
    j = magic_enum::enum_name(value);
};

void adl_serializer<gnev::ImageLoaderStbResult>::to_json(json& j,
                                                         const gnev::ImageLoaderStbResult&
                                                             value) {
    adl_serializer<gnev::OperationResult>::to_json(j, value);

    j["__class__"] = "ImageLoaderStbResult";
    j["msgs"] = value.messages;
};

} // namespace nlohmann