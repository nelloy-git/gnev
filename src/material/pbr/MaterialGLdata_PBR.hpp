#pragma once

#include <array>
#include <iostream>
#include <limits>

#include "glm/glm.hpp"
#include "nlohmann/json_fwd.hpp"
#include "util/Export.hpp"

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

struct EXPORT alignas(16) MaterialGLdata_PBR final {
public:
    static constexpr unsigned int TexSize = 5;
    static constexpr unsigned int InvalidIndex = std::numeric_limits<unsigned int>::max();

    MaterialGLdata_PBR();

    std::array<unsigned int, TexSize> tex_index;
    std::array<glm::vec4, TexSize> tex_offset;
    std::array<glm::vec4, TexSize> tex_multiplier;

    static std::size_t OffsetOfTexIndex(MaterialTexType_PBR type);
    static std::size_t OffsetOfTexOffset(MaterialTexType_PBR type);
    static std::size_t OffsetOfTexMultiplier(MaterialTexType_PBR type);
};

EXPORT std::ostream& operator<<(std::ostream& out, const MaterialGLdata_PBR& val);

} // namespace gnev

namespace nlohmann {

template <>
struct EXPORT adl_serializer<glm::vec4> {
    static void to_json(json& j, const glm::vec4& vec);
};

template <>
struct EXPORT adl_serializer<gnev::MaterialGLdata_PBR> {
    static void to_json(json& j, const gnev::MaterialGLdata_PBR& material);
};

} // namespace nlohmann