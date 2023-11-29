#pragma once

#include <initializer_list>
#include <memory>

#include "material/base/Material.hpp"
#include "material/base/MaterialStorage.hpp"
#include "util/Ref.hpp"

namespace gnev::base {

template <IsMaterial M>
class EXPORT MaterialFactory {
public:
    using Data = M::Data;
    static constexpr GLuint TexSize = M::TexSize;

    MaterialFactory(Ref<MaterialStorage<Data>> storage);
    virtual ~MaterialFactory();

    Ref<MaterialStorage<Data>> getStorage() const;

    virtual M create() = 0;

private:
    Ref<MaterialStorage<Data>> storage;
};

template <IsMaterial M>
MaterialFactory<M>::MaterialFactory(Ref<MaterialStorage<Data>> storage)
    : storage(storage) {}

template <IsMaterial M>
MaterialFactory<M>::~MaterialFactory() {}

template <IsMaterial M>
Ref<MaterialStorage<typename M::Data>> MaterialFactory<M>::getStorage() const {
    return storage;
}

} // namespace gnev::base