#include "transform/3d/TransformGL_3D.hpp"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace gnev {

std::ostream& operator<<(std::ostream& out, const glm::mat4& value) {
    out << json(value);
    return out;
}

std::ostream& operator<<(std::ostream& out, const TransformGL_3D& value) {
    out << json(value);
    return out;
}

} // namespace gnev

namespace nlohmann {

void adl_serializer<glm::mat4>::to_json(json& json, const glm::mat4& val) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            float v = val[i][j];
            json[4 * i + j] = v;
        }
    }
}

void adl_serializer<gnev::TransformGL_3D>::to_json(json& j,
                                                   const gnev::TransformGL_3D& val) {
    j["__class__"] = "TransformGL_3D";
    if (val.parent != gnev::TransformGL_3D::InvalidIndex) {
        j["parent"] = val.parent;
    } else {
        j["parent"] = nullptr;
    }
    j["mat"] = val.mat;
}

} // namespace nlohmann