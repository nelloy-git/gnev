#include "material/image_loader/MaterialImageLoaderStbiResult.hpp"

#include "magic_enum.hpp"
#include "nlohmann/json.hpp"

namespace gnev {

MaterialImageLoaderStbiResult::MaterialImageLoaderStbiResult(std::future<bool>&& done)
    : done(std::move(done)) {}

MaterialImageLoaderStbiResult::~MaterialImageLoaderStbiResult(){};

OperationStatus MaterialImageLoaderStbiResult::getStatus() const {
    auto s = done.wait_for(std::chrono::seconds(0));
    if (s == std::future_status::timeout){
        return OperationStatus::InProgress;
    }
    
    try {
        if (done.get()){
            return OperationStatus::Done;
        } else {
            return OperationStatus::Failed;
        }
    } catch (...){
        return OperationStatus::Failed;
    }
}

std::ostream& operator<<(std::ostream& out, const MaterialImageLoaderStbiResult& value) {
    out << nlohmann::json(value);
    return out;
}

} // namespace gnev

namespace nlohmann {

void adl_serializer<gnev::MaterialImageLoaderStbiResult::Message>::
    to_json(json& j, const gnev::MaterialImageLoaderStbiResult::Message& value) {
    j = magic_enum::enum_name(value);
};

void adl_serializer<gnev::MaterialImageLoaderStbiResult>::
    to_json(json& j, const gnev::MaterialImageLoaderStbiResult& value) {
    adl_serializer<gnev::OperationResult>::to_json(j, value);

    j["__class__"] = "MaterialImageLoaderStbiResult";
    j["msgs"] = value.messages;
};

} // namespace nlohmann