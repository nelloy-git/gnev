#pragma once

#include <array>
#include <iostream>
#include <limits>

#include "glm/glm.hpp"
#include "material/base/MaterialGL.hpp"

namespace gnev {

enum class MaterialTexType_PBR : base::MaterialTexIndex {
    Albedo = 0,
    Normal = 1,
    Metallic = 2,
    Roughness = 3,
    AmbientOcclusion = 4,
};

inline constexpr base::MaterialTexIndex toUint(MaterialTexType_PBR type) {
    return static_cast<base::MaterialTexIndex>(type);
}

struct EXPORT alignas(16) MaterialGL_PBR {
public:
    static constexpr base::MaterialTexIndex TexSize = 5;

    inline static constexpr base::MaterialTexIndex SizeOfTexOffsetElem() {
        return sizeof(decltype(tex_offset)::value_type);
    }

    inline static constexpr base::MaterialTexIndex SizeOfTexMultiplierElem() {
        return sizeof(decltype(tex_multiplier)::value_type);
    }

    inline static constexpr base::MaterialTexIndex
    OffsetOfTexOffset(MaterialTexType_PBR type) {
        return offsetof(MaterialGL_PBR, tex_offset) +
               toUint(type) * SizeOfTexOffsetElem();
    }

    inline static constexpr base::MaterialTexIndex
    OffsetOfTexMultiplier(MaterialTexType_PBR type) {
        return offsetof(MaterialGL_PBR, tex_multiplier) +
               toUint(type) * SizeOfTexMultiplierElem();
    }

    MaterialGL_PBR() {
        std::fill_n(tex_index.begin(),
                    TexSize,
                    std::numeric_limits<base::MaterialTexIndex>::max());
        std::fill_n(tex_offset.begin(), TexSize, glm::vec4{0, 0, 0, 0});
        std::fill_n(tex_multiplier.begin(), TexSize, glm::vec4{1, 1, 1, 1});
    }

    std::array<base::MaterialTexIndex, TexSize> tex_index;
    std::array<glm::vec4, TexSize> tex_offset;
    std::array<glm::vec4, TexSize> tex_multiplier;
};

inline std::ostream& operator<<(std::ostream& out, const MaterialGL_PBR& val) {
    out << "{tex_index: [";
    for (auto index : val.tex_index) {
        out << index << ",";
    }
    out << "], tex_offset: [";
    for (const auto& offset : val.tex_offset) {
        out << "{";
        for (int i = 0; i < 4; ++i) {
            out << offset[i] << ",";
        }
        out << "},";
    }
    out << "], tex_multiplier: [";
    for (const auto& multiplier : val.tex_multiplier) {
        out << "{";
        for (int i = 0; i < 4; ++i) {
            out << multiplier[i] << ",";
        }
        out << "},";
    }
    out << "}";

    return out;
}

} // namespace gnev