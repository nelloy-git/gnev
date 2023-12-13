#include "gl/texture/IndexMapView.hpp"

namespace gnev::gl::texture {

IndexMapView::IndexMapView(const Ref<Accessor3d>& accessor)
    : IndexStorage(accessor->getDepth())
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
    // IndexMapView::change(index, info, changer);
}

} // namespace gnev::gl::texture