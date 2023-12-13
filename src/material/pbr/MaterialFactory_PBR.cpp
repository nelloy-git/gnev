#include "material/pbr/MaterialFactory_PBR.hpp"

#include <filesystem>

namespace gnev {

using DataView = MaterialFactory_PBR::DataView;
using TexView = MaterialFactory_PBR::TexView;

using DataElem = MaterialFactory_PBR::DataElem;
using TexElem = MaterialFactory_PBR::TexElem;

MaterialFactory_PBR::MaterialFactory_PBR(const Ref<DataView>& data_view,
                                         const std::array<Ref<TexView>, TexSize>&
                                             tex_views)
    : data_view(data_view)
    , tex_views(tex_views) {}

MaterialFactory_PBR::MaterialFactory_PBR(GLuint l, GLuint w, GLuint h, GLuint c)
    : data_view(initDefaultData(c))
    , tex_views({initDefaultTex(l, w, h, c),
                 initDefaultTex(l, w, h, c),
                 initDefaultTex(l, w, h, c),
                 initDefaultTex(l, w, h, c),
                 initDefaultTex(l, w, h, c)}) {}

MaterialFactory_PBR::~MaterialFactory_PBR(){};

Ref<DataView> MaterialFactory_PBR::getDataView() const { return data_view; }

Ref<TexView> MaterialFactory_PBR::getTexView(MaterialTexType_PBR type) const {
    return tex_views.at(toUint(type));
}

Ref<Material_PBR> MaterialFactory_PBR::createMaterial() {
    return MakeSharable<Material_PBR>(data_view);
}

Ref<TexElem> MaterialFactory_PBR::createTex(MaterialTexType_PBR type) {
    return MakeSharable<TexElem>(tex_views.at(toUint(type)));
}

Ref<DataView> MaterialFactory_PBR::initDefaultData(GLuint capacity) {
    auto buffer = MakeSharable<gl::Buffer>();
    buffer->initStorage(capacity * sizeof(MaterialGL_PBR),
                        nullptr,
                        GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
    auto accessor = MakeSharable<gl::buffer::AccessorSubData>(buffer);
    return MakeSharable<DataView>(accessor);
}

Ref<TexView> MaterialFactory_PBR::initDefaultTex(GLuint img_levels,
                                                 GLuint w,
                                                 GLuint img_height,
                                                 GLuint capacity) {
    auto tex = MakeSharable<gl::Texture>(GL_TEXTURE_2D_ARRAY);
    tex->initStorage3D(img_levels, GL_RGBA8, w, img_height, capacity);
    auto accessor = MakeSharable<gl::texture::Accessor3dSubImage>(tex);
    return MakeSharable<TexView>(accessor);
}

} // namespace gnev