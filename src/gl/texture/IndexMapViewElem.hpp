#pragma once

#include "gl/texture/IndexMapView.hpp"
#include "util/WeakRef.hpp"

namespace gnev::gl::texture {

class EXPORT IndexMapViewElem {
public:
    using Changer = IndexMapView::Changer;

    IndexMapViewElem(WeakRef<IndexMapView> weak_view);
    virtual ~IndexMapViewElem() = default;

    WeakRef<IndexMapView> getWeakView() const;
    Ref<IndexMapView> getView() const;
    Ref<GLuint> getIndex() const;

    void set(const Image& src);
    void get(Image& dst) const;
    void change(const ImageInfo& info, Changer changer);

private:
    WeakRef<IndexMapView> weak_view;
    Ref<GLuint> index_guard;

    static Ref<GLuint> initIndexGuard(const WeakRef<IndexMapView>& weak_view);
};

} // namespace gnev::gl::texture