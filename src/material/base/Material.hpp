#pragma once

#include <functional>
#include <initializer_list>
#include <memory>
#include <stdexcept>

#include "material/base/MaterialData.hpp"
#include "material/base/MaterialGL.hpp"
#include "material/base/MaterialStorage.hpp"
#include "material/base/MaterialTex.hpp"
#include "util/WeakRef.hpp"

namespace gnev::base {

template <IsMaterialGL T>
class EXPORT Material {
public:
    using Data = T;
    static constexpr GLuint TexSize = T::TexSize;

    Material(WeakRef<MaterialStorage<T>> weak_storage, const T& initial = T{});
    Material(const Material&) = delete;
    Material(Material&&) = default;
    virtual ~Material() = default;

    WeakRef<MaterialStorage<T>> getWeakStorage() const;
    Ref<MaterialData<T>> getDataRef() const;
    std::optional<Ref<MaterialTex>> getTexRef(GLuint type) const;
    void setTexRef(GLuint type, std::optional<Ref<MaterialTex>> tex_ref);

    // Helpers
    GLuint getIndex() const;

private:
    WeakRef<MaterialStorage<T>> weak_storage;

    Ref<MaterialData<T>> data_ref;
    std::array<std::shared_ptr<MaterialTex>, TexSize> tex_refs;

    static MaterialData<T> initData(WeakRef<MaterialStorage<T>> weak_storage);
};

namespace details {

template <typename derived>
struct is_Material {
    template <IsMaterialGL T>
    static constexpr std::true_type test(const Material<T>*);
    static constexpr std::false_type test(...);
    static constexpr bool value = decltype(test(std::declval<derived*>()))::value;
};

} // namespace details

template <typename T>
concept IsMaterial = details::is_Material<T>::value;

template <IsMaterialGL T>
Material<T>::Material(WeakRef<MaterialStorage<T>> weak_storage, const T& initial)
    : weak_storage(weak_storage)
    , data_ref(MakeSharable<MaterialData<T>>(weak_storage.lock().value()->data,
                                             initial)) {}

template <IsMaterialGL T>
WeakRef<MaterialStorage<T>> Material<T>::getWeakStorage() const {
    return weak_storage;
}

template <IsMaterialGL T>
GLuint Material<T>::getIndex() const {
    return data_ref->getIndex();
}

template <IsMaterialGL T>
Ref<MaterialData<T>> Material<T>::getDataRef() const {
    return data_ref;
}

template <IsMaterialGL T>
std::optional<Ref<MaterialTex>> Material<T>::getTexRef(GLuint index) const {
    return tex_refs[index] ? std::optional<Ref<MaterialTex>>(tex_refs[index])
                           : std::nullopt;
}

template <IsMaterialGL T>
void Material<T>::setTexRef(GLuint type, std::optional<Ref<MaterialTex>> tex_ref) {
    GLuint index =
        tex_ref.has_value() ? *tex_ref.value()->getIndex() : T::InvalidTexIndex;

    tex_refs.at(type) = tex_ref->getPtr();
    getDataRef()->template setData<GLuint>(&index,
                                           offsetof(T, tex_index) +
                                               type * sizeof(GLuint));
}

template <IsMaterialGL T>
MaterialData<T> Material<T>::initData(WeakRef<MaterialStorage<T>> weak_storage) {
    auto storage_opt = weak_storage.lock();
    if (not storage_opt.has_value()) {
        throw std::runtime_error("");
    }
    auto& storage = storage_opt.value();
    return MaterialData<T>(storage.data);
}

} // namespace gnev::base