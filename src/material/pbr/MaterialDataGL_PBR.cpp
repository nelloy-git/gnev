#include "material/pbr/MaterialDataGL_PBR.hpp"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace gnev {

std::size_t MaterialDataGL_PBR::getOffsetOfTexIndex(MaterialTexType_PBR type) {
    static constexpr std::size_t ElemSize =
        sizeof(decltype(MaterialDataGL_PBR::tex_index)::value_type);
    return offsetof(MaterialDataGL_PBR, tex_index) + toUint(type) * ElemSize;
}

std::size_t MaterialDataGL_PBR::getOffsetOfColorOffset(MaterialTexType_PBR type) {
    static constexpr std::size_t ElemSize =
        sizeof(decltype(MaterialDataGL_PBR::color_offset)::value_type);
    return offsetof(MaterialDataGL_PBR, color_offset) + toUint(type) * ElemSize;
}

std::size_t MaterialDataGL_PBR::getOffsetOfColorMult(MaterialTexType_PBR type) {
    static constexpr std::size_t ElemSize =
        sizeof(decltype(MaterialDataGL_PBR::color_mult)::value_type);
    return offsetof(MaterialDataGL_PBR, color_mult) + toUint(type) * ElemSize;
}

std::ostream& operator<<(std::ostream& out, const MaterialDataGL_PBR& value) {
    out << json(value);
    return out;
}

} // namespace gnev

namespace nlohmann {

void adl_serializer<glm::vec4>::to_json(json& j, const glm::vec4& value) {
    for (int i = 0; i < 4; ++i) {
        j[i] = value[i];
    }
};

void adl_serializer<gnev::MaterialDataGL_PBR>::to_json(json& j,
                                                       const gnev::MaterialDataGL_PBR&
                                                           value) {
    json tex_index = {};
    for (auto index : value.tex_index) {
        if (index == gnev::MaterialDataGL_PBR::InvalidIndex) {
            tex_index.push_back(nullptr);
        } else {
            tex_index.push_back(index);
        }
    }

    j["__class__"] = "Material_PBR";
    j["tex_index"] = tex_index;
    j["tex_offset"] = value.color_offset;
    j["tex_multiplier"] = value.color_mult;
};

} // namespace nlohmann
