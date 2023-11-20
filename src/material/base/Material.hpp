#pragma once

#include <bits/utility.h>
#include <functional>
#include <initializer_list>
#include <memory>
#include <stdexcept>

#include "material/base/Define.hpp"
#include "material/base/MaterialDataRef.hpp"
#include "material/base/MaterialGL.hpp"
#include "material/base/MaterialStorage.hpp"
#include "material/base/MaterialTexRef.hpp"

namespace gnev::base {

template <IsMaterialGL T>
class EXPORT Material {
public:
    static constexpr MaterialTexIndex TexSize = T::TexSize;

    Material(MaterialStorage<T>& storage);
    virtual ~Material();

    MaterialDataRef<T> getDataRef() const;

    std::optional<MaterialTexRef> getTexRef(MaterialDataIndex index) const;
    void setTexRef(MaterialTexIndex index, std::optional<MaterialTexRef> tex_ref);

private:
    MaterialDataRef<T> data_ref;
    std::array<std::optional<MaterialTexRef>, TexSize> tex_refs;
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
Material<T>::Material(MaterialStorage<T>& storage)
    : data_ref(storage.data_storage) {}

template <IsMaterialGL T>
Material<T>::~Material() {}

template <IsMaterialGL T>
MaterialDataRef<T> Material<T>::getDataRef() const {
    return data_ref;
}

template <IsMaterialGL T>
std::optional<MaterialTexRef> Material<T>::getTexRef(MaterialTexIndex index) const {
    return tex_refs[index];
}

template <IsMaterialGL T>
void Material<T>::setTexRef(MaterialTexIndex index,
                            std::optional<MaterialTexRef> tex_ref) {
    tex_refs[index] = tex_ref;
}

} // namespace gnev::base