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

struct EXPORT MaterialDataGL_PBR final {
public:
    static constexpr unsigned int TexSize = 5;
    static constexpr unsigned int InvalidIndex = std::numeric_limits<unsigned int>::max();

    alignas(16) std::array<unsigned int, TexSize> tex_index = {
        InvalidIndex, InvalidIndex, InvalidIndex, InvalidIndex, InvalidIndex};
    alignas(16) std::array<glm::vec4, TexSize> color_offset = {
        glm::vec4{0.f}, glm::vec4{0.f}, glm::vec4{0.f}, glm::vec4{0.f}, glm::vec4{0.f}};
    alignas(16) std::array<glm::vec4, TexSize> color_mult = {
        glm::vec4{1.f}, glm::vec4{1.f}, glm::vec4{1.f}, glm::vec4{1.f}, glm::vec4{1.f}};

    static std::size_t getOffsetOfTexIndex(MaterialTexType_PBR type);
    static std::size_t getOffsetOfColorOffset(MaterialTexType_PBR type);
    static std::size_t getOffsetOfColorMult(MaterialTexType_PBR type);
};

static_assert(offsetof(MaterialDataGL_PBR, tex_index) == 0,
              "offsetof(MaterialGLdata_PBR, tex_index) != 0");
static_assert(offsetof(MaterialDataGL_PBR, color_offset) == 32,
              "offsetof(MaterialTexType_PBR, color_offset) != 32");
static_assert(offsetof(MaterialDataGL_PBR, color_mult) == 112,
              "offsetof(MaterialGLdata_PBR, color_mult) != 112");

EXPORT std::ostream& operator<<(std::ostream& out, const MaterialDataGL_PBR& val);

} // namespace gnev

namespace nlohmann {

template <>
struct EXPORT adl_serializer<glm::vec4> {
    static void to_json(json& j, const glm::vec4& vec);
};

template <>
struct EXPORT adl_serializer<gnev::MaterialDataGL_PBR> {
    static void to_json(json& j, const gnev::MaterialDataGL_PBR& material);
};

} // namespace nlohmann