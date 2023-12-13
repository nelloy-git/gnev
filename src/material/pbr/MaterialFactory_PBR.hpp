#pragma once

#include "material/pbr/MaterialGL_PBR.hpp"
#include "material/pbr/Material_PBR.hpp"
#include "util/Ref.hpp"

namespace gnev {

class EXPORT MaterialFactory_PBR {
public:
    using DataView = Material_PBR::DataView;
    using TexView = Material_PBR::TexView;

    using DataElem = Material_PBR::DataElem;
    using TexElem = Material_PBR::TexElem;

    static constexpr GLuint TexSize = MaterialGL_PBR::TexSize;

    MaterialFactory_PBR(const Ref<DataView>& data_view,
                        const std::array<Ref<TexView>, TexSize>& tex_views);
    MaterialFactory_PBR(GLuint img_levels,
                        GLuint img_width,
                        GLuint img_height,
                        GLuint capacity);
    virtual ~MaterialFactory_PBR();

    Ref<DataView> getDataView() const;
    Ref<TexView> getTexView(MaterialTexType_PBR type) const;

    Ref<Material_PBR> createMaterial();
    Ref<TexElem> createTex(MaterialTexType_PBR type);

private:
    Ref<DataView> data_view;
    std::array<Ref<TexView>, TexSize> tex_views;

    static Ref<DataView> initDefaultData(GLuint capacity);
    static Ref<TexView> initDefaultTex(GLuint img_levels,
                                       GLuint img_width,
                                       GLuint img_height,
                                       GLuint capacity);
};

} // namespace gnev