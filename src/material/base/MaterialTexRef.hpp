#pragma once

#include <memory>

#include "material/base/MaterialTexStorage.hpp"

namespace gnev::base {

class EXPORT MaterialTexRef {
public:
    MaterialTexRef(const std::weak_ptr<MaterialTexRefStorage>& weak_storage);
    virtual ~MaterialTexRef();

    const std::weak_ptr<MaterialTexRefStorage>& weak_storage;
    const std::shared_ptr<const MaterialTexIndex> index;

private:
    static std::shared_ptr<const MaterialTexIndex>
    init(const std::weak_ptr<MaterialTexRefStorage>& weak_storage);
};

} // namespace gnev::base