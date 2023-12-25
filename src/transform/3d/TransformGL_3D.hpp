#pragma once

#include <limits>

#include "glm/glm.hpp"
#include "nlohmann/json_fwd.hpp"
#include "transform/base/TransformGL.hpp"
#include "util/Export.hpp"

namespace gnev {

struct EXPORT alignas(16) TransformGL_3D {
    static constexpr unsigned int InvalidIndex = std::numeric_limits<unsigned int>::max();

    unsigned int parent = InvalidIndex;
    glm::mat4 mat = glm::mat4(1.f);
};

EXPORT std::ostream& operator<<(std::ostream& out, const TransformGL_3D& val);
EXPORT std::ostream& operator<<(std::ostream& out, const glm::mat4& val);

} // namespace gnev

namespace nlohmann {

template <>
struct EXPORT adl_serializer<glm::mat4> {
    static void to_json(json& j, const glm::mat4& val);
};

template <>
struct EXPORT adl_serializer<gnev::TransformGL_3D> {
    static void to_json(json& j, const gnev::TransformGL_3D& val);
};

} // namespace nlohmann