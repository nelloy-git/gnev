#pragma once

#include <concepts>

#include "gl/buffer/accessor/IBufferAccessor.hpp"
#include "util/IndexStorage.hpp"
#include "util/InstanceLogger.hpp"

namespace gnev::gl {

template <typename T>
concept IsBufferViewMapData =
    not std::is_pointer_v<T> and std::is_trivially_constructible_v<T>;

template <typename K, IsBufferViewMapData V>
class BufferViewMap {
public:
    template <IsBufferViewMapData V>
    using Changer = std::function<void(V&)>;

    BufferViewMap(const Ref<IBufferAccessor>& accessor)
        : accessor{accessor}
        , capacity{accessor->getBuffer()->getSize() / sizeof(T)}
        , index_storage{capacity} {}

    bool hasKey(const K& key) const { return key2index.contains(key); }


private:
    Ref<IBufferAccessor> accessor;

    unsigned capacity;
    std::unordered_map<K, unsigned> key2index;
    IndexStorage index_storage;

    bool reserveIndex(const K& key) {
        auto index_opt = index_storage.useIndex();
        if (not index_opt.has_value()) {
            return false;
        }
        key2index[key] = index_opt.value();
        return true;
    }
};

} // namespace gnev::gl