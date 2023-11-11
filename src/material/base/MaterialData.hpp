#pragma once

#include <array>

#include "glm/glm.hpp"
#include "util/Util.hpp"

namespace gnev::base {

template <unsigned int TexN>
    requires(TexN > 0)
struct EXPORT alignas(16) MaterialData {
public:
    using Index = unsigned int;

    std::array<Index, TexN> tex_index;
    std::array<glm::vec4, TexN> tex_offset;
    std::array<glm::vec4, TexN> tex_scale;
};

} // namespace gnev::tool