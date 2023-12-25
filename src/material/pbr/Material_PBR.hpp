#pragma once

#include "gl/buffer/IndexMapViewElem.hpp"
#include "gl/texture/IndexMapViewElem.hpp"
#include "material/base/Material.hpp"
#include "material/pbr/MaterialGLdata_PBR.hpp"

namespace gnev {

class EXPORT Material_PBR : public Material {
public:
    using GLdata = MaterialGLdata_PBR;

    template <typename V>
    using Changer = gl::buffer::IndexMapViewElem<GLdata>::Changer<V>;

    using DataView = gl::buffer::IndexMapView<GLdata>;
    using TexView = gl::texture::IndexMapView;

    using DataElem = gl::buffer::IndexMapViewElem<GLdata>;
    using TexElem = gl::texture::IndexMapViewElem;

    Material_PBR(WeakRef<DataView> weak_storage, const GLdata& initial = GLdata{});
    virtual ~Material_PBR() = default;

    GLuint getIndex() const override;
    GLdata getGLdata() const;

    void setTex(MaterialTexType_PBR type, Ptr<TexElem> tex);
    Ptr<TexElem> getTex(MaterialTexType_PBR type) const;

    void setTexOffset(MaterialTexType_PBR type, const glm::vec4& value);
    glm::vec4 getTexOffset(MaterialTexType_PBR type) const;
    void changeTexOffset(MaterialTexType_PBR type, const Changer<glm::vec4>& changer);

    void setTexMultiplier(MaterialTexType_PBR type, const glm::vec4& value);
    glm::vec4 getTexMultiplier(MaterialTexType_PBR type) const;
    void changeTexMultiplier(MaterialTexType_PBR type, const Changer<glm::vec4>& changer);

private:
    Ref<DataElem> data;
    std::array<Ptr<TexElem>, GLdata::TexSize> textures;
};

} // namespace gnev