#include "material/pbr/MaterialStorage_PBR.hpp"

namespace gnev {

using DataView = MaterialStorage_PBR::DataView;
using TexView = MaterialStorage_PBR::TexView;

Ref<MaterialStorage_PBR> MaterialStorage_PBR::MakeDynamic(GLuint img_levels,
                                                          GLuint img_width,
                                                          GLuint img_height,
                                                          GLuint capacity) {
    auto data_view = DataView::MakeDynamic(capacity);
    auto tex_view_albedo =
        TexView::MakeArray2D(img_levels, img_width, img_height, capacity);
    auto tex_view_normal =
        TexView::MakeArray2D(img_levels, img_width, img_height, capacity);
    auto tex_view_metallic =
        TexView::MakeArray2D(img_levels, img_width, img_height, capacity);
    auto tex_view_roughness =
        TexView::MakeArray2D(img_levels, img_width, img_height, capacity);
    auto tex_view_ambient_occlusion =
        TexView::MakeArray2D(img_levels, img_width, img_height, capacity);

    return MakeSharable<MaterialStorage_PBR>(data_view,
                                             tex_view_albedo,
                                             tex_view_normal,
                                             tex_view_metallic,
                                             tex_view_roughness,
                                             tex_view_ambient_occlusion);
}

MaterialStorage_PBR::MaterialStorage_PBR(const Ref<DataView>& data_view,
                                         const Ref<TexView>& tex_view_albedo,
                                         const Ref<TexView>& tex_view_normal,
                                         const Ref<TexView>& tex_view_metallic,
                                         const Ref<TexView>& tex_view_roughness,
                                         const Ref<TexView>& tex_view_ambient_occlusion)
    : data_view{data_view}
    , tex_view{tex_view_albedo,
               tex_view_normal,
               tex_view_metallic,
               tex_view_roughness,
               tex_view_ambient_occlusion} {}

Ref<DataView> MaterialStorage_PBR::getDataView() const { return data_view; }

Ref<TexView> MaterialStorage_PBR::getTexView(MaterialTexType_PBR type) const {
    return tex_view.at(toUint(type));
}

Ref<gl::Texture> MaterialStorage_PBR::getTex(MaterialTexType_PBR type) const {
    return getTexView(type)->accessor->texture;
}

} // namespace gnev