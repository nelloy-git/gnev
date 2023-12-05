#include "util/OperationResult.hpp"

#include "magic_enum.hpp"
#include "nlohmann/json.hpp"

namespace nlohmann {

void adl_serializer<gnev::OperationResult>::to_json(json& j,
                                                    const gnev::OperationResult& val) {
    j["__class__"] = "OperationResult";
    j["status"] = magic_enum::enum_name(val.getStatus());
}

} // namespace nlohmann