#pragma once

#include "material/pbr/Material_PBR.hpp"

namespace gnev {

class EXPORT MaterialStorage_PBR {
public:
    using DataGL = MaterialDataGL_PBR;
    using DataView = Material_PBR::DataView;
    using DataElem = Material_PBR::DataElem;

    using TexView = Material_PBR::TexView;
    using TexElem = Material_PBR::TexElem;

    static Ref<MaterialStorage_PBR>
    MakeDynamic(GLuint img_levels, GLuint img_width, GLuint img_height, GLuint capacity);

    MaterialStorage_PBR(const Ref<DataView>& material_storage,
                        const Ref<TexView>& albedo_storage,
                        const Ref<TexView>& normal_storage,
                        const Ref<TexView>& metallic_storage,
                        const Ref<TexView>& roughness_storage,
                        const Ref<TexView>& ambient_occlusion_storage);
    virtual ~MaterialStorage_PBR() = default;

    Ref<DataView> getDataView() const;
    Ref<TexView> getTexView(MaterialTexType_PBR type) const;
    Ref<gl::Texture> getTex(MaterialTexType_PBR type) const;

private:
    Ref<DataView> data_view;
    std::array<Ref<TexView>, MaterialDataGL_PBR::TexSize> tex_view;
};
} // namespace gnev