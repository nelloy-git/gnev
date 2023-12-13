#pragma once

#include "gl/buffer/IndexMapView.hpp"
#include "util/WeakRef.hpp"

namespace gnev::gl::buffer {

template <typename T>
class EXPORT IndexMapViewElem {
public:
    template <typename V>
    using Changer = IndexMapView<T>::template Changer<V>;

    IndexMapViewElem(WeakRef<IndexMapView<T>> weak_view, const T& initial = T{});
    virtual ~IndexMapViewElem() = default;

    WeakRef<IndexMapView<T>> getWeakView() const;
    Ref<IndexMapView<T>> getView() const;
    Ref<GLuint> getIndex() const;

    void set(const T& src);

    template <typename V>
    void set(const V& src, GLintptr ptr_offset);

    void get(T& dst) const;

    template <typename V>
    void get(V& dst, GLintptr ptr_offset) const;

    void change(const Changer<T>& changer);

    template <typename V>
    void change(const Changer<V>& changer, GLintptr ptr_offset);

private:
    WeakRef<IndexMapView<T>> weak_view;
    Ref<GLuint> index_guard;

    static Ref<GLuint> initIndexGuard(const WeakRef<IndexMapView<T>>& weak_view,
                                      const T& initial);
};

template <typename T>
IndexMapViewElem<T>::IndexMapViewElem(WeakRef<IndexMapView<T>> weak_view,
                                      const T& initial)
    : weak_view(weak_view)
    , index_guard(initIndexGuard(weak_view, initial)) {
    set(initial);
}

template <typename T>
WeakRef<IndexMapView<T>> IndexMapViewElem<T>::getWeakView() const {
    return weak_view;
}

template <typename T>
Ref<IndexMapView<T>> IndexMapViewElem<T>::getView() const {
    auto view_opt = weak_view.lock();
    if (not view_opt.has_value()) {
        throw std::runtime_error("");
    }
    return view_opt.value();
}

template <typename T>
Ref<GLuint> IndexMapViewElem<T>::getIndex() const {
    return index_guard;
}

template <typename T>
void IndexMapViewElem<T>::set(const T& src) {
    getView()->set(index_guard, src);
}

template <typename T>
template <typename V>
void IndexMapViewElem<T>::set(const V& src, GLintptr ptr_offset) {
    getView()->set(index_guard, src, ptr_offset);
}

template <typename T>
void IndexMapViewElem<T>::get(T& dst) const {
    getView()->get(index_guard, dst);
}

template <typename T>
template <typename V>
void IndexMapViewElem<T>::get(V& dst, GLintptr ptr_offset) const {
    getView()->get(index_guard, dst, ptr_offset);
}

template <typename T>
void IndexMapViewElem<T>::change(const Changer<T>& changer) {
    getView()->change(index_guard, changer);
}

template <typename T>
template <typename V>
void IndexMapViewElem<T>::change(const Changer<V>& changer, GLintptr ptr_offset) {
    getView()->change(index_guard, changer, ptr_offset);
}

template <typename T>
Ref<GLuint> IndexMapViewElem<T>::initIndexGuard(const WeakRef<IndexMapView<T>>& weak_view,
                                                const T& initial) {
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

} // namespace gnev::gl::buffer