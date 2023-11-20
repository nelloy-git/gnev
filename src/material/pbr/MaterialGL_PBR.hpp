#pragma once

#include <array>

#include "glm/glm.hpp"
#include "material/base/MaterialGL.hpp"

namespace gnev {

static constexpr unsigned int PBR_TEX_N = 5;

enum class MaterialTexRefType_PBR : unsigned int {
    Albedo = 0,
    Normal = 1,
    Metallic = 2,
    Roughness = 3,
    AmbientOcclusion = 4,
};
static constexpr base::MaterialTexIndex TEX_SIZE_PBR = 5;

struct EXPORT alignas(16) MaterialGL_PBR : public base::MaterialGL<5> {
public:
    MaterialGL_PBR()
        : base::MaterialGL<5>() {
        std::fill_n(tex_offset.begin(), TEX_SIZE_PBR, glm::vec4{0, 0, 0, 0});
        std::fill_n(tex_multiplier.begin(), TEX_SIZE_PBR, glm::vec4{1, 1, 1, 1});
    }

    std::array<glm::vec4, TEX_SIZE_PBR> tex_offset;
    std::array<glm::vec4, TEX_SIZE_PBR> tex_multiplier;
};

} // namespace gnev