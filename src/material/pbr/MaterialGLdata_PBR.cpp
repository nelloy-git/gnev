#include "material/pbr/MaterialGLdata_PBR.hpp"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace gnev {

MaterialGLdata_PBR::MaterialGLdata_PBR() {
    std::fill_n(tex_index.begin(), TexSize, InvalidIndex);
    std::fill_n(tex_offset.begin(), TexSize, glm::vec4{0, 0, 0, 0});
    std::fill_n(tex_multiplier.begin(), TexSize, glm::vec4{1, 1, 1, 1});
}

std::size_t MaterialGLdata_PBR::OffsetOfTexIndex(MaterialTexType_PBR type) {
    static constexpr std::size_t ElemSize =
        sizeof(decltype(MaterialGLdata_PBR::tex_index)::value_type);
    return offsetof(MaterialGLdata_PBR, tex_index) + toUint(type) * ElemSize;
}

std::size_t MaterialGLdata_PBR::OffsetOfTexOffset(MaterialTexType_PBR type) {
    static constexpr std::size_t ElemSize =
        sizeof(decltype(MaterialGLdata_PBR::tex_offset)::value_type);
    return offsetof(MaterialGLdata_PBR, tex_offset) + toUint(type) * ElemSize;
}

std::size_t MaterialGLdata_PBR::OffsetOfTexMultiplier(MaterialTexType_PBR type) {
    static constexpr std::size_t ElemSize =
        sizeof(decltype(MaterialGLdata_PBR::tex_multiplier)::value_type);
    return offsetof(MaterialGLdata_PBR, tex_multiplier) +
           toUint(type) * ElemSize;
}

std::ostream& operator<<(std::ostream& out, const MaterialGLdata_PBR& value) {
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

void adl_serializer<gnev::MaterialGLdata_PBR>::to_json(json& j,
                                                       const gnev::MaterialGLdata_PBR&
                                                           value) {
    json tex_index = {};
    for (auto index : value.tex_index) {
        if (index == gnev::MaterialGLdata_PBR::InvalidIndex) {
            tex_index.push_back(nullptr);
        } else {
            tex_index.push_back(index);
        }
    }

    j["__class__"] = "Material_PBR";
    j["tex_index"] = tex_index;
    j["tex_offset"] = value.tex_offset;
    j["tex_multiplier"] = value.tex_multiplier;
};

} // namespace nlohmann
