#pragma once

#include "gl/buffer/IndexMapViewElem.hpp"
#include "gl/texture/IndexMapViewElem.hpp"
#include "material/pbr/MaterialDataGL_PBR.hpp"

namespace gnev {

class MaterialStorage_PBR;

class EXPORT Material_PBR {
public:
    using DataGL = MaterialDataGL_PBR;
    using DataView = gl::buffer::IndexMapView<DataGL>;
    using DataElem = gl::buffer::WeakIndexMapViewElem<DataGL>;

    using TexView = gl::texture::IndexMapView;
    using TexElem = gl::texture::WeakIndexMapViewElem;

    Material_PBR(const Ref<MaterialStorage_PBR>& material_storage);
    virtual ~Material_PBR() = default;

    WeakRef<MaterialStorage_PBR> getWeakStorage() const;
    Ref<MaterialStorage_PBR> getStorage() const;
    Ref<DataElem> getData() const;

    void setTex(MaterialTexType_PBR type, Ptr<TexElem> tex);
    Ptr<TexElem> getTex(MaterialTexType_PBR type) const;

    void setTexOffset(MaterialTexType_PBR type, const glm::vec4& value);
    glm::vec4 getTexOffset(MaterialTexType_PBR type) const;

    void setTexMultiplier(MaterialTexType_PBR type, const glm::vec4& value);
    glm::vec4 getTexMultiplier(MaterialTexType_PBR type) const;

private:
    WeakRef<MaterialStorage_PBR> weak_storage;

    Ref<DataElem> data;
    std::array<Ptr<TexElem>, MaterialDataGL_PBR::TexSize> tex;
};

} // namespace gnev