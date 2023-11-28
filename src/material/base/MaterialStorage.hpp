#pragma once

#include <array>

#include "gl/buffer/BufStorageIndexMap.hpp"
#include "gl/texture/TexStorageIndexMap.hpp"
#include "material/base/MaterialDataStorage.hpp"
#include "material/base/MaterialGL.hpp"
#include "material/base/MaterialTexStorage.hpp"
#include "util/Export.hpp"
#include "util/StrongRef.hpp"

namespace gnev::base {

template <IsMaterialGL T>
class EXPORT MaterialStorage {
public:
    static constexpr GLuint TexSize = T::TexSize;

    MaterialStorage(StrongRef<MaterialDataStorage<T>> data,
                    const std::array<StrongRef<MaterialTexStorage>, TexSize>& textures);
    virtual ~MaterialStorage();

    const StrongRef<MaterialDataStorage<T>> data;
    const std::array<StrongRef<MaterialTexStorage>, TexSize> textures;
};

template <IsMaterialGL T>
MaterialStorage<T>::MaterialStorage(StrongRef<MaterialDataStorage<T>> data,
                                    const std::array<StrongRef<MaterialTexStorage>,
                                                     TexSize>& tex_storages)
    : data(data)
    , textures(tex_storages) {}

template <IsMaterialGL T>
MaterialStorage<T>::~MaterialStorage() {}

} // namespace gnev::base