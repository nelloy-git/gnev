#pragma once

#include "material/base/Material.hpp"
#include "material/base/MaterialTex.hpp"
#include "material/pbr/MaterialGL_PBR.hpp"

namespace gnev {

class Material_PBR : public base::Material<MaterialGL_PBR> {
public:
    Material_PBR(WeakRef<base::MaterialStorage<MaterialGL_PBR>> weak_storage,
                 const MaterialGL_PBR& initial = MaterialGL_PBR{});
    Material_PBR(const Material_PBR&) = delete;
    Material_PBR(Material_PBR&&) = default;
    ~Material_PBR();

    std::optional<base::MaterialTex> getTexRef(MaterialTexType_PBR type) const;
    void
    setTexRef(MaterialTexType_PBR type, std::optional<Ref<base::MaterialTex>> tex_ref);

    void setTexOffset(MaterialTexType_PBR type, const glm::vec4& value);
    glm::vec4 getTexOffset(MaterialTexType_PBR type) const;

    void setTexMultiplier(MaterialTexType_PBR type, const glm::vec4& value);
    glm::vec4 getTexMultiplier(MaterialTexType_PBR type) const;
};

} // namespace gnev