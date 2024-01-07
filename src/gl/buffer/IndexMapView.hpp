#pragma once

#include "gl/buffer/Accessor.hpp"
#include "util/IndexStorage.hpp"
#include "util/Ref.hpp"

namespace gnev::gl::buffer {

template <typename T>
concept UsableByIndexMapView = std::is_trivially_copyable_v<T>;

template <UsableByIndexMapView T>
class EXPORT IndexMapView : public IndexStorage {
public:
    using Data = T;
    template <UsableByIndexMapView V>
    using Changer = std::function<void(V&)>;

    static Ref<IndexMapView> MakeDynamic(GLuint capacity);
    static Ref<IndexMapView> MakeCoherent(GLuint capacity);

    IndexMapView(const Ref<gl::buffer::Accessor>& accessor);
    virtual ~IndexMapView() = default;

    Ref<Accessor> accessor;

    void set(GLuint index, const T& src);

    template <UsableByIndexMapView V>
    void set(GLuint index, const V& src, GLintptr ptr_offset);

    void get(GLuint index, T& dst) const;

    template <UsableByIndexMapView V>
    void get(GLuint index, V& dst, GLintptr ptr_offset) const;

    void change(GLuint index, const Changer<T>& changer);

    template <UsableByIndexMapView V>
    void change(GLuint index, const Changer<V>& changer, GLintptr ptr_offset);
};

template <UsableByIndexMapView T>
Ref<IndexMapView<T>> IndexMapView<T>::MakeDynamic(GLuint capacity) {
    auto buffer = MakeSharable<gl::Buffer>();
    buffer->initStorage(capacity * sizeof(T), nullptr, GL_DYNAMIC_STORAGE_BIT);
    auto accessor = MakeSharable<gl::buffer::AccessorSubData>(buffer);
    return MakeSharable<IndexMapView<T>>(accessor);
}

template <UsableByIndexMapView T>
Ref<IndexMapView<T>> IndexMapView<T>::MakeCoherent(GLuint capacity) {
    auto buffer = MakeSharable<gl::Buffer>();
    buffer->initStorage(capacity * sizeof(T),
                        nullptr,
                        GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT |
                            GL_MAP_COHERENT_BIT);
    auto accessor = MakeSharable<gl::buffer::AccessorCoherent>(buffer);
    return MakeSharable<IndexMapView<T>>(accessor);
}

template <UsableByIndexMapView T>
IndexMapView<T>::IndexMapView(const Ref<gl::buffer::Accessor>& accessor)
    : IndexStorage(accessor->buffer->getSize() / sizeof(T))
    , accessor(accessor) {}

template <UsableByIndexMapView T>
void IndexMapView<T>::set(GLuint index, const T& src) {
    if (not isUsed(index)) {
        throw std::out_of_range("");
    }
    accessor->set(index * sizeof(T), sizeof(T), &src);
}

template <UsableByIndexMapView T>
template <UsableByIndexMapView V>
void IndexMapView<T>::set(GLuint index, const V& src, GLintptr ptr_offset) {
    if (not isUsed(index) or (ptr_offset + sizeof(V) >= sizeof(T))) {
        throw std::out_of_range("");
    }
    accessor->set(index * sizeof(T) + ptr_offset, sizeof(V), &src);
}

template <UsableByIndexMapView T>
void IndexMapView<T>::get(GLuint index, T& dst) const {
    if (not isUsed(index)) {
        throw std::out_of_range("");
    }
    accessor->get(index * sizeof(T), sizeof(T), &dst);
}

template <UsableByIndexMapView T>
template <UsableByIndexMapView V>
void IndexMapView<T>::get(GLuint index, V& dst, GLintptr ptr_offset) const {
    if (not isUsed(index) or (ptr_offset + sizeof(V) >= sizeof(T))) {
        throw std::out_of_range("");
    }
    accessor->get(index * sizeof(T) + ptr_offset, sizeof(V), &dst);
}

template <UsableByIndexMapView T>
void IndexMapView<T>::change(GLuint index, const Changer<T>& changer) {
    if (not isUsed(index)) {
        throw std::out_of_range("");
    }
    auto wrapper = [this, &changer](Accessor&, void* ptr, GLintptr size) {
        changer(*this, *static_cast<T*>(ptr));
    };
    accessor->change(index * sizeof(T), sizeof(T), wrapper);
}

template <UsableByIndexMapView T>
template <UsableByIndexMapView V>
void IndexMapView<T>::change(GLuint index,
                             const Changer<V>& changer,
                             GLintptr ptr_offset) {
    if (not isUsed(index) or (ptr_offset + sizeof(V) >= sizeof(T))) {
        throw std::out_of_range("");
    }
    auto wrapper = [this, &changer](void* ptr, GLintptr size) {
        changer(*static_cast<V*>(ptr));
    };
    accessor->change(index * sizeof(T) + ptr_offset, sizeof(V), wrapper);
}

} // namespace gnev::gl::buffer