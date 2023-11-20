#pragma once

#include <filesystem>

#include "material/base/Material.hpp"
#include "material/pbr/MaterialGL_PBR.hpp"
#include "material/pbr/MaterialStorage_PBR.hpp"

namespace gnev {

class Material_PBR : public base::Material<MaterialGL_PBR> {
public:
    Material_PBR(const std::weak_ptr<MaterialStorage_PBR>& weak_storage);
    Material_PBR(const Material_PBR&) = delete;
    Material_PBR(Material_PBR&&) = default;
    ~Material_PBR();

    std::weak_ptr<MaterialStorage_PBR> getWeakStorage() const;
    std::shared_ptr<MaterialStorage_PBR> lockStorage() const;

    std::optional<base::MaterialTexRef> getTexRef(MaterialTexType_PBR type) const;
    void setTexRef(MaterialTexType_PBR type, std::optional<base::MaterialTexRef> tex_ref);

    std::shared_ptr<base::MaterialImageLoader::Result>
    loadTex(MaterialTexType_PBR type,
            base::MaterialImageLoader& loader,
            const std::filesystem::path& path,
            const gl::TexImageInfo& info);

    void setTexOffset(MaterialTexType_PBR type, const glm::vec4& value);
    glm::vec4 getTexOffset(MaterialTexType_PBR type) const;

    void setTexMultiplier(MaterialTexType_PBR type, const glm::vec4& value);
    glm::vec4 getTexMultiplier(MaterialTexType_PBR type) const;

private:
};

} // namespace gnev