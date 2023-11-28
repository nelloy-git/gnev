#pragma once

#include <bits/utility.h>
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
    virtual ~Material();

    WeakRef<MaterialStorage<T>> getWeakStorage() const;
    StrongRef<MaterialData<T>> getDataRef() const;
    std::optional<StrongRef<MaterialTex>> getTexRef(GLuint type) const;
    void setTexRef(GLuint type, std::optional<StrongRef<MaterialTex>> tex_ref);

private:
    WeakRef<MaterialStorage<T>> weak_storage;

    StrongRef<MaterialData<T>> data_ref;
    std::array<std::optional<StrongRef<MaterialTex>>, TexSize> tex_refs;

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
    , data_ref(StrongRef<MaterialData<T>>::Make(weak_storage.lock().value()->data,
                                                initial)) {}

template <IsMaterialGL T>
Material<T>::~Material() {}

template <IsMaterialGL T>
WeakRef<MaterialStorage<T>> Material<T>::getWeakStorage() const {
    return weak_storage;
}

template <IsMaterialGL T>
StrongRef<MaterialData<T>> Material<T>::getDataRef() const {
    return data_ref;
}

template <IsMaterialGL T>
std::optional<StrongRef<MaterialTex>> Material<T>::getTexRef(GLuint index) const {
    return tex_refs[index];
}

template <IsMaterialGL T>
void Material<T>::setTexRef(GLuint type, std::optional<StrongRef<MaterialTex>> tex_ref) {
    GLuint index = tex_ref.has_value() ? tex_ref.value()->getIndex() : T::InvalidTexIndex;

    tex_refs[index] = tex_ref;
    getDataRef()->template setData<GLuint>(&index,
                                           offsetof(T, tex_index) +
                                               index * sizeof(GLuint));
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