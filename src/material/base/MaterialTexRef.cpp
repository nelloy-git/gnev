#include "material/base/MaterialTexRef.hpp"

#include "material/base/Define.hpp"

namespace gnev::base {

MaterialTexRef::MaterialTexRef(const std::weak_ptr<MaterialTexRefStorage>& weak_storage)
    : weak_storage(weak_storage)
    , index(init(weak_storage)) {}

MaterialTexRef::~MaterialTexRef() {}

std::shared_ptr<const MaterialTexIndex>
MaterialTexRef::init(const std::weak_ptr<MaterialTexRefStorage>& weak_storage) {
    auto storage = weak_storage.lock();
    if (!storage) {
        throw std::runtime_error("");
    }

    auto index_opt = storage->initIndex();
    if (not index_opt.has_value()) {
        throw std::out_of_range("");
    }

    auto p_index = new MaterialTexIndex(index_opt.value());
    auto del = [weak_storage](MaterialTexIndex* p_index) {
        auto storage = weak_storage.lock();
        if (storage) {
            storage->freeIndex(*p_index);
        }
        delete p_index;
    };

    return std::shared_ptr<const MaterialTexIndex>(p_index, del);
}

} // namespace gnev::base