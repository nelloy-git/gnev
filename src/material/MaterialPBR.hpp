#pragma once

#include "material/MaterialDataPBR.hpp"
#include "material/base/Material.hpp"

namespace gnev {

class MaterialPBR : public base::Material<MaterialDataPBR, PBR_TEX_N> {
public:
    virtual std::optional<TextureView> getTexture(Index tex_n);
    virtual std::optional<const TextureView> getTexture(Index tex_n) const;
};
} // namespace gnev