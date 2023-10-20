#pragma once

#include <array>
#include <limits>

#include "glm/glm.hpp"
#include "util/Util.hpp"

namespace gnev::tool {

template <std::size_t TexN>
    requires(TexN >= 3)
class EXPORT alignas(16) MaterialData final {
    template <std::size_t>
    friend class MaterialMap;

public:
    using Index = unsigned int;
    static constexpr Index RESERVED_INDEX = std::numeric_limits<Index>::max();

    MaterialData() { textures.fill(RESERVED_INDEX); };

    ~MaterialData() = default;

    struct Param {
        glm::vec4 mult = {1, 1, 1, 1};
        glm::vec4 add = {0, 0, 0, 0};
    };

    Index self_index = RESERVED_INDEX;
    std::array<Index, TexN> textures;
    std::array<Param, TexN> params;
};

} // namespace gnev::tool