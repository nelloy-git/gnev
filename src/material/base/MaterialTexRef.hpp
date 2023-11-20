#pragma once

#include <memory>

#include "material/base/Define.hpp"
#include "util/Export.hpp"

namespace gnev::base {

class MaterialTexStorage;

class EXPORT MaterialTexRef {
public:
    MaterialTexRef(const std::weak_ptr<MaterialTexStorage>& weak_storage);
    virtual ~MaterialTexRef();

    std::weak_ptr<MaterialTexStorage> getWeakStorage() const;
    std::shared_ptr<MaterialTexStorage> lockStorage() const;
    std::shared_ptr<const MaterialTexIndex> getIndex() const;

private:
    std::weak_ptr<MaterialTexStorage> weak_storage;
    std::shared_ptr<const MaterialTexIndex> index;

    static std::shared_ptr<const MaterialTexIndex>
    init(const std::weak_ptr<MaterialTexStorage>& weak_storage);
};

} // namespace gnev::base