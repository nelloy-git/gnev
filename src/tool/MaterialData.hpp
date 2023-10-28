#pragma once

#include <array>
#include <limits>

#include "glm/glm.hpp"
#include "util/Util.hpp"



namespace gnev::tool {

template <std::size_t TexN>
struct EXPORT alignas(16) MaterialData {
public:
    

    struct Param {
        glm::vec4 offset = {0, 0, 0, 0};
        glm::vec4 scale = {1, 1, 1, 1};
    };

    std::size_t self_index = RESERVED_INDEX;
    std::array<std::size_t, TexN> textures;
    std::array<Param, TexN> params;
};

} // namespace gnev::tool