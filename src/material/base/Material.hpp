#pragma once

#include <stdexcept>

#include "gl/buffer/IndexMapViewElem.hpp"
#include "gl/texture/IndexMapViewElem.hpp"
#include "material/base/MaterialGL.hpp"

namespace gnev::base {

template <IsMaterialGL T>
class EXPORT Material {
public:
    using DataView = gl::buffer::IndexMapView<T>;
    using TexView = gl::texture::IndexMapView;

    using DataElem = gl::buffer::IndexMapViewElem<T>;
    using TexElem = gl::texture::IndexMapViewElem;

    static constexpr GLuint InvalidIndex = T::InvalidIndex;
    static constexpr GLuint TexSize = T::TexSize;

    Material(WeakRef<DataView> weak_view, const T& initial = T{});
    virtual ~Material() = default;

protected:
    Ref<DataElem> data;
    std::array<Ptr<TexElem>, TexSize> textures;

    void setTex(GLuint type, Ptr<TexElem> tex);
    Ptr<TexElem> getTex(GLuint type) const;
};

template <IsMaterialGL T>
Material<T>::Material(WeakRef<DataView> weak_view, const T& initial)
    : data(MakeSharable<DataElem>(weak_view, initial)) {}

template <IsMaterialGL T>
void Material<T>::setTex(GLuint type, Ptr<TexElem> tex) {
    textures[type] = tex;
    GLuint index = tex ? *tex->getIndex() : InvalidIndex;
    data->set(index, offsetof(T, tex_index) + type * sizeof(GLuint));
}

template <IsMaterialGL T>
Ptr<typename Material<T>::TexElem> Material<T>::getTex(GLuint type) const {
    return textures[type];
}

} // namespace gnev::base