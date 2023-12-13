#include "gl/texture/IndexMapViewElem.hpp"

namespace gnev::gl::texture {

IndexMapViewElem::IndexMapViewElem(WeakRef<IndexMapView> weak_view)
    : weak_view(weak_view)
    , index_guard(initIndexGuard(weak_view)) {}

WeakRef<IndexMapView> IndexMapViewElem::getWeakView() const { return weak_view; }

Ref<IndexMapView> IndexMapViewElem::getView() const {
    auto view_opt = weak_view.lock();
    if (not view_opt.has_value()) {
        throw std::runtime_error("");
    }
    return view_opt.value();
}

Ref<GLuint> IndexMapViewElem::getIndex() const { return index_guard; }

void IndexMapViewElem::set(const Image& src) { getView()->set(index_guard, src); }

void IndexMapViewElem::get(Image& dst) const { getView()->get(index_guard, dst); }

void IndexMapViewElem::change(const ImageInfo& info, Changer changer) {
    getView()->change(index_guard, info, changer);
}

Ref<GLuint> IndexMapViewElem::initIndexGuard(const WeakRef<IndexMapView>& weak_view) {
    auto view_opt = weak_view.lock();
    if (not view_opt.has_value()) {
        throw std::runtime_error("");
    }

    auto index_opt = view_opt.value()->useIndex();
    if (not index_opt.has_value()) {
        throw std::runtime_error("");
    }
    auto index = index_opt.value();

    auto del = [weak_view](GLuint* p_index) {
        auto view_opt = weak_view.lock();
        if (view_opt.has_value()) {
            view_opt.value()->freeIndex(*p_index);
        }
        delete p_index;
    };

    auto shared_index = std::shared_ptr<GLuint>(new GLuint(index), del);
    return Ref<GLuint>(shared_index);
}

} // namespace gnev::gl::texture
