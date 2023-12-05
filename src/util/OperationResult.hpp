#pragma once

#include "util/Export.hpp"
#include "nlohmann/json_fwd.hpp"

namespace gnev {

enum class OperationStatus {
    Idle,
    InProgress,
    Done,
    Failed
};

class EXPORT OperationResult {
public:
    virtual OperationStatus getStatus() const = 0;
};

} // namespace gnev

namespace nlohmann {

template <>
struct EXPORT adl_serializer<gnev::OperationResult> {
    static void to_json(json& j, const gnev::OperationResult& val);
};

} // namespace nlohmann