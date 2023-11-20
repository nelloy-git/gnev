#pragma once

#include <memory>
#include <stdexcept>

#include "gl/buffer/BufStorageIterator.hpp"
#include "material/base/MaterialDataStorage.hpp"

namespace gnev::base {

template <IsMaterialGL T>
class EXPORT MaterialDataRef {
public:
    MaterialDataRef(const std::weak_ptr<MaterialDataStorage<T>>& weak_storage);
    virtual ~MaterialDataRef();

    std::weak_ptr<MaterialDataStorage<T>> getWeakStorage() const;
    std::shared_ptr<MaterialDataStorage<T>> lockStorage() const;
    std::shared_ptr<const MaterialTexIndex> getIndex() const;

    std::pair<std::shared_ptr<MaterialDataStorage<T>>, gl::BufStorageIterator<T>>
    lockIter() const;

private:
    std::weak_ptr<MaterialDataStorage<T>> weak_storage;
    std::shared_ptr<const MaterialDataIndex> index;

    static std::shared_ptr<const MaterialDataIndex>
    init(const std::weak_ptr<MaterialDataStorage<T>>& weak_storage);
};

template <IsMaterialGL T>
MaterialDataRef<T>::MaterialDataRef(const std::weak_ptr<MaterialDataStorage<T>>&
                                        weak_storage)
    : weak_storage(weak_storage)
    , index(init(weak_storage)) {}

template <IsMaterialGL T>
MaterialDataRef<T>::~MaterialDataRef() {}

template <IsMaterialGL T>
std::weak_ptr<MaterialDataStorage<T>> MaterialDataRef<T>::getWeakStorage() const {
    return weak_storage;
}

template <IsMaterialGL T>
std::shared_ptr<MaterialDataStorage<T>> MaterialDataRef<T>::lockStorage() const {
    return std::shared_ptr<MaterialDataStorage<T>>(weak_storage);
}

template <IsMaterialGL T>
std::shared_ptr<const MaterialTexIndex> MaterialDataRef<T>::getIndex() const {
    return index;
}

template <IsMaterialGL T>
std::pair<std::shared_ptr<MaterialDataStorage<T>>, gl::BufStorageIterator<T>>
MaterialDataRef<T>::lockIter() const {
    auto storage = lockStorage();
    return {storage, storage->at(*index)};
}

template <IsMaterialGL T>
std::shared_ptr<const MaterialDataIndex>
MaterialDataRef<T>::init(const std::weak_ptr<MaterialDataStorage<T>>& weak_storage) {
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

    return std::shared_ptr<const MaterialDataIndex>(p_index, del);
}

} // namespace gnev::base