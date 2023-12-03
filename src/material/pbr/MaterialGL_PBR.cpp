#include "material/pbr/MaterialGL_PBR.hpp"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace gnev {

MaterialGL_PBR::MaterialGL_PBR() {
    std::fill_n(tex_index.begin(), TexSize, InvalidTexIndex);
    std::fill_n(tex_offset.begin(), TexSize, glm::vec4{0, 0, 0, 0});
    std::fill_n(tex_multiplier.begin(), TexSize, glm::vec4{1, 1, 1, 1});
}

std::size_t MaterialGL_PBR::OffsetOfTexOffset(MaterialTexType_PBR type) {
    return offsetof(MaterialGL_PBR, tex_offset) + toUint(type) * SizeOfTexOffsetElem;
}

std::size_t MaterialGL_PBR::OffsetOfTexMultiplier(MaterialTexType_PBR type) {
    return offsetof(MaterialGL_PBR, tex_multiplier) +
           toUint(type) * SizeOfTexMultiplierElem;
}

std::ostream& operator<<(std::ostream& out, const MaterialGL_PBR& val) {
    out << json(val);
    return out;
}

} // namespace gnev

namespace nlohmann {

void adl_serializer<glm::vec4>::to_json(json& j, const glm::vec4& vec) {
    for (int i = 0; i < 4; ++i) {
        j[i] = vec[i];
    }
};

void adl_serializer<gnev::MaterialGL_PBR>::to_json(json& j,
                                                   const gnev::MaterialGL_PBR& material) {
    json tex_index = {};
    for (auto index : material.tex_index) {
        if (index == gnev::MaterialGL_PBR::InvalidTexIndex) {
            tex_index.push_back(nullptr);
        } else {
            tex_index.push_back(index);
        }
    }

    j["Material_PBR"] = {
        {"tex_index", tex_index},
        {"tex_offset", material.tex_offset},
        {"tex_multiplier", material.tex_multiplier},
    };
};

} // namespace nlohmann
