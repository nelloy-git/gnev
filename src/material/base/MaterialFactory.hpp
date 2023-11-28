#pragma once

#include <initializer_list>
#include <memory>

#include "material/base/Material.hpp"
#include "material/base/MaterialStorage.hpp"
#include "util/StrongRef.hpp"

namespace gnev::base {

template <IsMaterial M>
class EXPORT MaterialFactory {
public:
    using Data = M::Data;
    static constexpr GLuint TexSize = M::TexSize;

    MaterialFactory(StrongRef<MaterialStorage<Data>> storage);
    virtual ~MaterialFactory();

    StrongRef<MaterialStorage<Data>> getStorage() const;

    virtual M create() = 0;

private:
    StrongRef<MaterialStorage<Data>> storage;
};

template <IsMaterial M>
MaterialFactory<M>::MaterialFactory(StrongRef<MaterialStorage<Data>> storage)
    : storage(storage) {}

template <IsMaterial M>
MaterialFactory<M>::~MaterialFactory() {}

template <IsMaterial M>
StrongRef<MaterialStorage<typename M::Data>> MaterialFactory<M>::getStorage() const {
    return storage;
}

} // namespace gnev::base