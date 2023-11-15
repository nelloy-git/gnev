#pragma once

#include <array>
#include <limits>

#include "material/base/Defines.hpp"
#include "glm/glm.hpp"
#include "util/Util.hpp"

namespace gnev {

static constexpr unsigned int PBR_TEX_N = 5;

enum class ParamPBR : unsigned int {
    Albedo = 0,
    Normal = 1,
    Metallic = 2,
    Roughness = 3,
    AmbientOcclusion = 4,
};

struct EXPORT alignas(16) MaterialDataPBR {
public:
    static constexpr unsigned int TexN = 5;

    MaterialDataPBR(){
        std::fill_n(tex_index.begin(), TexN, std::numeric_limits<GLuint>::max());
        std::fill_n(tex_offset.begin(), TexN, glm::vec4{0, 0, 0, 0});
        std::fill_n(tex_multiplier.begin(), TexN, glm::vec4{1, 1, 1, 1});
    }

    std::array<GLuint, TexN> tex_index;
    std::array<glm::vec4, TexN> tex_offset;
    std::array<glm::vec4, TexN> tex_multiplier;
};

} // namespace gnev