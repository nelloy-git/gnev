#pragma once

#include <array>
#include <memory>

#include "material/base/MaterialDataStorage.hpp"
#include "material/base/MaterialGL.hpp"
#include "material/base/MaterialTexStorage.hpp"

namespace gnev::base {

template <IsMaterialGL T>
class EXPORT MaterialStorage {
public:
    static constexpr MaterialTexIndex TexSize = T::TexSize;

    MaterialStorage(const std::shared_ptr<MaterialDataStorage<T>>& data_storage,
                    const std::array<const std::shared_ptr<MaterialTexStorage>, TexSize>&
                        tex_storages);
    virtual ~MaterialStorage();

    const std::shared_ptr<MaterialDataStorage<T>> data_storage;
    const std::array<const std::shared_ptr<MaterialTexStorage>, TexSize> tex_storages;
};

template <IsMaterialGL T>
MaterialStorage<
    T>::MaterialStorage(const std::shared_ptr<MaterialDataStorage<T>>& data_storage,
                        const std::array<const std::shared_ptr<MaterialTexStorage>,
                                         TexSize>& tex_storages)
    : data_storage(data_storage)
    , tex_storages(tex_storages) {}

template <IsMaterialGL T>
MaterialStorage<T>::~MaterialStorage() {}

} // namespace gnev::base