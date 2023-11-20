#pragma once

#include <filesystem>

#include "material/base/Material.hpp"
#include "material/pbr/MaterialGL_PBR.hpp"
#include "material/pbr/MaterialStorage_PBR.hpp"

namespace gnev {

class Material_PBR : public base::Material<MaterialGL_PBR> {
public:
    Material_PBR(MaterialStorage_PBR& storage);
    ~Material_PBR();

    void setTextureIndex(MaterialTexRefType_PBR type, GLuint value);
    GLuint getTextureIndex(MaterialTexRefType_PBR type) const;

    void uploadTexture(MaterialTexRefType_PBR type,
                       const std::filesystem::path& path,
                       GLuint level);

    void setTextureOffset(MaterialTexRefType_PBR type, const glm::vec4& value);
    glm::vec4 getTextureOffset(MaterialTexRefType_PBR type) const;

    void setTextureMultiplier(MaterialTexRefType_PBR type, const glm::vec4& value);
    glm::vec4 getTextureMultiplier(MaterialTexRefType_PBR type) const;
};
} // namespace gnev