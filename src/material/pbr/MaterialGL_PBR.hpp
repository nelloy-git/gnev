#pragma once

#include <array>
#include <iostream>
#include <limits>

#include "glm/glm.hpp"
#include "material/base/MaterialGL.hpp"

namespace gnev {

enum class MaterialTexType_PBR : unsigned int {
    Albedo = 0,
    Normal = 1,
    Metallic = 2,
    Roughness = 3,
    AmbientOcclusion = 4,
};

inline constexpr unsigned int toUint(MaterialTexType_PBR type) {
    return static_cast<unsigned int>(type);
}

struct EXPORT alignas(16) MaterialGL_PBR {
public:
    static constexpr unsigned int TexSize = 5;
    static constexpr unsigned int InvalidTexIndex =
        std::numeric_limits<unsigned int>::max();

    inline static constexpr std::size_t SizeOfTexOffsetElem() {
        return sizeof(decltype(tex_offset)::value_type);
    }

    inline static constexpr std::size_t SizeOfTexMultiplierElem() {
        return sizeof(decltype(tex_multiplier)::value_type);
    }

    inline static constexpr std::size_t
    OffsetOfTexOffset(MaterialTexType_PBR type) {
        return offsetof(MaterialGL_PBR, tex_offset) +
               toUint(type) * SizeOfTexOffsetElem();
    }

    inline static constexpr std::size_t
    OffsetOfTexMultiplier(MaterialTexType_PBR type) {
        return offsetof(MaterialGL_PBR, tex_multiplier) +
               toUint(type) * SizeOfTexMultiplierElem();
    }

    MaterialGL_PBR() {
        std::fill_n(tex_index.begin(), TexSize, InvalidTexIndex);
        std::fill_n(tex_offset.begin(), TexSize, glm::vec4{0, 0, 0, 0});
        std::fill_n(tex_multiplier.begin(), TexSize, glm::vec4{1, 1, 1, 1});
    }

    std::array<unsigned int, TexSize> tex_index;
    std::array<glm::vec4, TexSize> tex_offset;
    std::array<glm::vec4, TexSize> tex_multiplier;
};

inline std::ostream& operator<<(std::ostream& out, const glm::vec4& val) {
    static constexpr auto VEC4 = "vec4";

    out << VEC4 << "{";
    out << val[0];
    for (int i = 1; i < 4; ++i) {
        out << "," << val[i];
    }
    out << "}";
    return out;
}

inline std::ostream& operator<<(std::ostream& out, const MaterialGL_PBR& val) {
    static constexpr auto INVALID_INDEX_NAME = "-";

    static constexpr auto MATERIAL_PBR = "Material_PBR";
    static constexpr auto TEX_INDEX = "index";
    static constexpr auto TEX_OFFSET = "offset";
    static constexpr auto TEX_MULT = "mult";

    out << MATERIAL_PBR << "{";

    out << TEX_INDEX << ": [";
    if (val.tex_index[0] == MaterialGL_PBR::InvalidTexIndex) {
        out << INVALID_INDEX_NAME;
    } else {
        out << val.tex_index[0];
    }
    for (int i = 1; i < val.tex_index.size(); ++i) {
        out << ", ";
        if (val.tex_index[i] == MaterialGL_PBR::InvalidTexIndex) {
            out << INVALID_INDEX_NAME;
        } else {
            out << val.tex_index[i];
        }
    }
    out << "]";

    out << ", " << TEX_OFFSET << ": [";
    out << val.tex_offset[0];
    for (int i = 1; i < val.tex_offset.size(); ++i) {
        out << ", " << val.tex_offset[i];
    }
    out << "]";

    out << ", " << TEX_MULT << ": [";
    out << val.tex_multiplier[0];
    for (int i = 1; i < val.tex_multiplier.size(); ++i) {
        out << ", " << val.tex_multiplier[i];
    }
    out << "]";
    out << "}";

    return out;
}

} // namespace gnev