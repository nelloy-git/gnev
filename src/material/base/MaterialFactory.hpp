#pragma once

#include <initializer_list>
#include <memory>

#include "material/base/Material.hpp"
#include "material/base/MaterialStorage.hpp"

namespace gnev::base {

template <IsMaterial M>
class EXPORT MaterialFactory {
public:
    using Data = M::Data;
    static constexpr MaterialTexIndex TexSize = M::TexSize;

    MaterialFactory(const std::shared_ptr<MaterialStorage<Data>>& storage);
    virtual ~MaterialFactory();

    const std::shared_ptr<MaterialStorage<Data>> storage;

    virtual M create() = 0;

private:
};

template <IsMaterial M>
MaterialFactory<M>::MaterialFactory(const std::shared_ptr<MaterialStorage<Data>>& storage)
    : storage(storage) {}

template <IsMaterial M>
MaterialFactory<M>::~MaterialFactory() {}

} // namespace gnev::base