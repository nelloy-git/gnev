#pragma once

#include "material/base/MaterialData.hpp"

namespace gnev {

static constexpr unsigned int PBR_TEX_N = 5;

enum class ParamPBR : unsigned int {
    Albedo = 0,
    Normal = 1,
    Metallic = 2,
    Roughness = 3,
    AmbientOcclusion = 4,
};

struct EXPORT alignas(16) MaterialDataPBR : protected base::MaterialData<PBR_TEX_N> {
public:
    Index getIndex();

    void setOffset(ParamPBR param, const glm::vec4& value);
    const glm::vec4& getOffset(ParamPBR param);

    void setScale(ParamPBR param, const glm::vec4& value);
    const glm::vec4& getScale(ParamPBR param);

    void setTexIndex(ParamPBR param, std::size_t value);
    std::size_t getTexIndex(ParamPBR param);

};

}