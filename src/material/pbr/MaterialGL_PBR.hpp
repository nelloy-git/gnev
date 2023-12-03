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

    MaterialGL_PBR();

    std::array<unsigned int, TexSize> tex_index;
    std::array<glm::vec4, TexSize> tex_offset;
    std::array<glm::vec4, TexSize> tex_multiplier;

    static constexpr std::size_t SizeOfTexOffsetElem =
        sizeof(decltype(MaterialGL_PBR::tex_offset)::value_type);
    static constexpr std::size_t SizeOfTexMultiplierElem =
        sizeof(decltype(MaterialGL_PBR::tex_multiplier)::value_type);

    static std::size_t OffsetOfTexOffset(MaterialTexType_PBR type);
    static std::size_t OffsetOfTexMultiplier(MaterialTexType_PBR type);
};

std::ostream& operator<<(std::ostream& out, const MaterialGL_PBR& val);

} // namespace gnev