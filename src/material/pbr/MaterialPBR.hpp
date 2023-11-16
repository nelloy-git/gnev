#pragma once

#include "material/base/Material.hpp"
#include "material/pbr/MaterialDataPBR.hpp"

namespace gnev {

class MaterialStoragePBR;

class MaterialPBR : public base::Material<MaterialDataPBR> {
public:
    using Base = base::Material<MaterialDataPBR>;
    using Storage = base::MaterialStorage<MaterialDataPBR>;

    MaterialPBR(const std::shared_ptr<Storage>& storage, GLuint data_index);
    ~MaterialPBR();

    void setTextureIndex(ParamPBR param, GLuint value);
    GLuint getTextureIndex(ParamPBR param) const;

    void setTextureOffset(ParamPBR param, const glm::vec4& value);
    glm::vec4 getTextureOffset(ParamPBR param) const;

    void setTextureMultiplier(ParamPBR param, const glm::vec4& value);
    glm::vec4 getTextureMultiplier(ParamPBR param) const;
};
} // namespace gnev