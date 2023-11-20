#pragma once

#include <bits/utility.h>
#include <functional>
#include <initializer_list>
#include <memory>
#include <stdexcept>

#include "material/base/Define.hpp"
#include "material/base/MaterialDataRef.hpp"
#include "material/base/MaterialGL.hpp"
#include "material/base/MaterialImageLoader.hpp"
#include "material/base/MaterialStorage.hpp"
#include "material/base/MaterialTexRef.hpp"

namespace gnev::base {

template <IsMaterialGL T>
class EXPORT Material {
public:
    using Data = T;
    static constexpr MaterialTexIndex TexSize = T::TexSize;

    Material(const std::weak_ptr<MaterialStorage<T>>& weak_storage);
    Material(const Material&) = delete;
    Material(Material&&) = default;
    virtual ~Material();

    std::weak_ptr<MaterialStorage<T>> getWeakStorage() const;
    std::shared_ptr<MaterialStorage<T>> lockStorage() const;

    MaterialDataRef<T> getDataRef() const;
    std::optional<MaterialTexRef> getTexRef(MaterialDataIndex index) const;
    void setTexRef(MaterialTexIndex index, std::optional<MaterialTexRef> tex_ref);

    std::shared_ptr<MaterialImageLoader::Result>
    loadTex(MaterialTexIndex index,
            MaterialImageLoader& loader,
            const std::filesystem::path& path,
            const gl::TexImageInfo& info);

private:
    std::weak_ptr<MaterialStorage<T>> weak_storage;

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
Material<T>::Material(const std::weak_ptr<MaterialStorage<T>>& weak_storage)
    : weak_storage(weak_storage)
    , data_ref(weak_storage.lock()->data_storage) {}

template <IsMaterialGL T>
Material<T>::~Material() {}

template <IsMaterialGL T>
std::weak_ptr<MaterialStorage<T>> Material<T>::getWeakStorage() const {
    return weak_storage;
}

template <IsMaterialGL T>
std::shared_ptr<MaterialStorage<T>> Material<T>::lockStorage() const {
    return std::shared_ptr<MaterialStorage<T>>(weak_storage);
}

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
    auto [storage, iter] = getDataRef().lockIter();
    iter.copyFrom(tex_ref->getIndex().get(),
                  offsetof(T, tex_index) + index * sizeof(MaterialTexIndex),
                  sizeof(MaterialTexIndex));
    tex_refs[index] = tex_ref;
}

template <IsMaterialGL T>
std::shared_ptr<MaterialImageLoader::Result>
Material<T>::loadTex(MaterialTexIndex index,
                     MaterialImageLoader& loader,
                     const std::filesystem::path& path,
                     const gl::TexImageInfo& info) {
    auto p_storage = weak_storage.lock();
    if (not p_storage) {
        throw std::runtime_error("");
    }

    auto tex_storage = p_storage->tex_storages[index];
    auto result = loader.upload(tex_storage, path, info);
    setTexRef(index, result->tex_ref);
    return result;
}

} // namespace gnev::base