#pragma once

#include <array>

#include "material/base/MaterialDataStorage.hpp"
#include "material/base/MaterialGL.hpp"
#include "material/base/MaterialTexStorage.hpp"
#include "util/Export.hpp"
#include "util/Ref.hpp"

namespace gnev::base {

template <IsMaterialGL T>
class EXPORT MaterialStorage {
public:
    static constexpr GLuint TexSize = T::TexSize;

    MaterialStorage(Ref<MaterialDataStorage<T>> data,
                    const std::array<Ref<MaterialTexStorage>, TexSize>& textures);
    virtual ~MaterialStorage();

    const Ref<MaterialDataStorage<T>> data;
    const std::array<Ref<MaterialTexStorage>, TexSize> textures;
};

template <IsMaterialGL T>
MaterialStorage<T>::MaterialStorage(Ref<MaterialDataStorage<T>> data,
                                    const std::array<Ref<MaterialTexStorage>, TexSize>&
                                        tex_storages)
    : data(data)
    , textures(tex_storages) {}

template <IsMaterialGL T>
MaterialStorage<T>::~MaterialStorage() {}

} // namespace gnev::base