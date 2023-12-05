#pragma once

#include "gl/BufferIndexMapView.hpp"
#include "util/WeakRef.hpp"

namespace gnev::gl {

template <typename T>
class EXPORT BufferIndexMapViewElement {
public:
    template <typename V>
    using Changer = BufferIndexMapView<T>::template Changer<V>;

    BufferIndexMapViewElement(WeakRef<BufferIndexMapView<T>> weak_view, const T& initial = T{});
    virtual ~BufferIndexMapViewElement() = default;

    WeakRef<BufferIndexMapView<T>> getWeakStorage() const;
    Ref<GLuint> getIndex() const;

    bool set(const T& src);

    template <typename V>
    bool set(const V& src, GLintptr ptr_offset);

    bool get(T& dst) const;

    template <typename V>
    bool get(V& dst, GLintptr ptr_offset) const;

    bool change(const Changer<T>& changer);

    template <typename V>
    bool change(const Changer<V>& changer, GLintptr ptr_offset);

private:
    WeakRef<BufferIndexMapView<T>> weak_view;
    Ref<GLuint> index_guard;

    static Ref<GLuint>
    initIndexKeeper(const WeakRef<BufferIndexMapView<T>>& weak_view,
                    const T& initial);
};

template <typename T>
BufferIndexMapViewElement<T>::BufferIndexMapViewElement(WeakRef<BufferIndexMapView<T>> weak_view,
                              const T& initial)
    : weak_view(weak_view)
    , index_guard(initIndexKeeper(weak_view, initial)) {
    set(initial);
}

template <typename T>
WeakRef<BufferIndexMapView<T>> BufferIndexMapViewElement<T>::getWeakStorage() const {
    return weak_view;
}

template <typename T>
Ref<GLuint> BufferIndexMapViewElement<T>::getIndex() const {
    return index_guard;
}

template <typename T>
bool BufferIndexMapViewElement<T>::set(const T& src) {
    auto view_opt = weak_view.lock();
    if (not view_opt.has_value()) {
        throw std::runtime_error("");
    }
    return view_opt.value()->set(index_guard, src);
}

template <typename T>
template <typename V>
bool BufferIndexMapViewElement<T>::set(const V& src, GLintptr ptr_offset) {
    auto view_opt = weak_view.lock();
    if (not view_opt.has_value()) {
        throw std::runtime_error("");
    }
    return view_opt.value()->set(index_guard, src, ptr_offset);
}

template <typename T>
bool BufferIndexMapViewElement<T>::get(T& dst) const {
    auto view_opt = weak_view.lock();
    if (not view_opt.has_value()) {
        throw std::runtime_error("");
    }
    return view_opt.value()->get(index_guard, dst);
}

template <typename T>
template <typename V>
bool BufferIndexMapViewElement<T>::get(V& dst, GLintptr ptr_offset) const {
    auto view_opt = weak_view.lock();
    if (not view_opt.has_value()) {
        throw std::runtime_error("");
    }
    return view_opt.value()->get(index_guard, dst, ptr_offset);
}

template <typename T>
bool BufferIndexMapViewElement<T>::change(const Changer<T>& changer) {
    auto view_opt = weak_view.lock();
    if (not view_opt.has_value()) {
        throw std::runtime_error("");
    }
    return view_opt.value()->set(index_guard, changer);
}

template <typename T>
template <typename V>
bool BufferIndexMapViewElement<T>::change(const Changer<V>& changer, GLintptr ptr_offset) {
    auto view_opt = weak_view.lock();
    if (not view_opt.has_value()) {
        throw std::runtime_error("");
    }
    return view_opt.value()->set(index_guard, changer, ptr_offset);
}

template <typename T>
Ref<GLuint>
BufferIndexMapViewElement<T>::initIndexKeeper(const WeakRef<BufferIndexMapView<T>>& weak_view,
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

} // namespace gnev::base