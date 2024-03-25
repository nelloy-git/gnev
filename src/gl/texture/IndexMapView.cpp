#include "gl/texture/IndexMapView.hpp"

namespace gnev::gl::texture {

Ref<IndexMapView> IndexMapView::MakeArray2D(GLuint img_levels,
                                            GLuint img_width,
                                            GLuint img_height,
                                            GLuint capacity) {
    auto tex = MakeSharable<gl::Texture>(TextureTarget::TEXTURE_2D_ARRAY);
    tex->initStorage3D(img_levels,
                       TextureInternalFormat::RGBA8,
                       img_width,
                       img_height,
                       capacity);
    auto accessor = MakeSharable<gl::texture::Accessor3dSubImage>(tex);
    return MakeSharable<IndexMapView>(accessor);
}

IndexMapView::IndexMapView(const Ref<Accessor3d>& accessor)
    : IndexManager(accessor->texture->getDepth(0))
    , accessor(accessor) {}

void IndexMapView::set(GLuint index, const Image& src) {
    if (not isUsed(index)) {
        throw std::out_of_range("");
    }
    accessor->set(Image3d{ImageInfo3d{index, src.info}, src.data});
}

void IndexMapView::get(GLuint index, Image& dst) const {
    Image3d dst3d{ImageInfo3d{index, dst.info}, dst.data};
    accessor->get(dst3d);
    dst.data = dst3d.data;
}

void IndexMapView::change(GLuint index, const ImageInfo& info, Changer changer) {
    accessor->change(ImageInfo3d{index, info}, changer);
}

} // namespace gnev::gl::texture