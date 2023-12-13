#pragma once

#include "material/base/Material.hpp"
#include "material/pbr/MaterialGL_PBR.hpp"

namespace gnev {

class EXPORT Material_PBR : public base::Material<MaterialGL_PBR> {
public:
    template <typename V>
    using Changer = gl::buffer::IndexMapViewElem<MaterialGL_PBR>::Changer<V>;

    using DataView = base::Material<MaterialGL_PBR>::DataView;
    using TexView = base::Material<MaterialGL_PBR>::TexView;

    using DataElem = base::Material<MaterialGL_PBR>::DataElem;
    using TexElem = base::Material<MaterialGL_PBR>::TexElem;

    Material_PBR(WeakRef<DataView> weak_storage,
                 const MaterialGL_PBR& initial = MaterialGL_PBR{});
    virtual ~Material_PBR() = default;

    MaterialGL_PBR get() const;

    void setTex(MaterialTexType_PBR type, Ptr<TexElem> tex);
    Ptr<TexElem> getTex(MaterialTexType_PBR type) const;

    void setTexOffset(MaterialTexType_PBR type, const glm::vec4& value);
    glm::vec4 getTexOffset(MaterialTexType_PBR type) const;
    void changeTexOffset(MaterialTexType_PBR type, const Changer<glm::vec4>& changer);

    void setTexMultiplier(MaterialTexType_PBR type, const glm::vec4& value);
    glm::vec4 getTexMultiplier(MaterialTexType_PBR type) const;
    void changeTexMultiplier(MaterialTexType_PBR type, const Changer<glm::vec4>& changer);
};

} // namespace gnev