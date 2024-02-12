#pragma once

#include <concepts>

#include "gl/buffer/accessor/IBufferAccessor.hpp"
#include "util/IndexManager.hpp"
#include "util/InstanceLogger.hpp"

namespace gnev::gl {

template <typename T>
concept IsBufferViewMapData =
    not std::is_pointer_v<T> and std::is_trivially_constructible_v<T>;

template <IsBufferViewMapData V>
class BufferViewMap {
public:
    template <IsBufferViewMapData T>
    using Changer = std::function<void(T&)>;

    BufferViewMap(const Ref<IBufferAccessor>& accessor)
        : accessor{accessor}
        , index_manager(IndexManager::Make(accessor->getBuffer()->getSize() / sizeof(V))){}

    auto reserve(unsigned count){
        auto res = 


        return index_manager->reserve(count, const FreeCallback &deleter_callback)
    }


private:
    Ref<IBufferAccessor> accessor;
    Ref<IndexManager> index_manager;

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