#pragma once

#include <stdexcept>

#include "material/base/MaterialDataStorage.hpp"
#include "material/base/MaterialGL.hpp"
#include "util/Export.hpp"
#include "util/Ref.hpp"
#include "util/WeakRef.hpp"

namespace gnev::base {

template <IsMaterialGL T>
class EXPORT MaterialData {
public:
    MaterialData(WeakRef<MaterialDataStorage<T>> weak_storage, const T& initial = T{});
    virtual ~MaterialData();

    WeakRef<MaterialDataStorage<T>> getWeakStorage() const;
    Ref<GLuint> getIndex() const;

    template <typename V>
    bool setData(const V* src, GLuint offset, GLuint size = sizeof(V));
    template <typename V>
    bool getData(V* dst, GLuint offset, GLuint size = sizeof(V)) const;

private:
    WeakRef<MaterialDataStorage<T>> weak_storage;
    Ref<GLuint> index_keeper;

    static Ref<GLuint>
    initIndexKeeper(const WeakRef<MaterialDataStorage<T>>& weak_storage,
                    const T& initial);
};

template <IsMaterialGL T>
MaterialData<T>::MaterialData(WeakRef<MaterialDataStorage<T>> weak_storage,
                              const T& initial)
    : weak_storage(weak_storage)
    , index_keeper(initIndexKeeper(weak_storage, initial)) {
    setData<T>(&initial, 0);
}

template <IsMaterialGL T>
MaterialData<T>::~MaterialData() {}

template <IsMaterialGL T>
WeakRef<MaterialDataStorage<T>> MaterialData<T>::getWeakStorage() const {
    return weak_storage;
}

template <IsMaterialGL T>
Ref<GLuint> MaterialData<T>::getIndex() const {
    return index_keeper;
}

template <IsMaterialGL T>
template <typename V>
bool MaterialData<T>::setData(const V* src, GLuint offset, GLuint size) {
    auto storage_opt = weak_storage.lock();
    if (not storage_opt.has_value()) {
        return false;
    }
    return storage_opt.value()->template setData<V>(index_keeper, src, offset, size);
}

template <IsMaterialGL T>
template <typename V>
bool MaterialData<T>::getData(V* dst, GLuint offset, GLuint size) const {
    auto storage_opt = weak_storage.lock();
    if (not storage_opt.has_value()) {
        return false;
    }
    return storage_opt.value()->template getData<V>(index_keeper, dst, offset, size);
}

template <IsMaterialGL T>
Ref<GLuint>
MaterialData<T>::initIndexKeeper(const WeakRef<MaterialDataStorage<T>>& weak_storage,
                                 const T& initial) {
    auto storage_opt = weak_storage.lock();
    if (not storage_opt.has_value()) {
        throw std::runtime_error("");
    }

    GLuint index = storage_opt.value()->initIndex();
    auto del = [weak_storage](GLuint* p_index) {
        auto storage_opt = weak_storage.lock();
        if (storage_opt.has_value()) {
            storage_opt.value()->freeIndex(*p_index);
        }
        delete p_index;
    };

    auto shared_index = std::shared_ptr<GLuint>(new GLuint(index), del);
    return Ref<GLuint>(shared_index);
}

} // namespace gnev::base