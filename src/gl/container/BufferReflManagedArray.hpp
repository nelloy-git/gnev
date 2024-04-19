#pragma once

#include <memory>

#include "gl/container/BufferReflArray.hpp"
#include "util/Export.hpp"
#include "util/PoolManager.hpp"

namespace gnev::gl {

template <IsTriviallyCopyable T>
class EXPORT BufferReflManagedArray : public BufferReflArray<T> {
public:
    using Range = PoolManager::Range;

    BufferReflManagedArray(std::unique_ptr<IBufferRawAccessor>&& accessor_)
        : BufferReflArray<T>{std::move(accessor_)}
        , manager{std::make_shared<PoolManager>(BufferReflArray<T>::capacity())} {}

    BufferReflManagedArray(const BufferReflManagedArray&) = delete;
    BufferReflManagedArray(BufferReflManagedArray&&) = default;
    virtual ~BufferReflManagedArray() = default;

    inline std::optional<std::size_t> reserveIndex() {
        return manager->reserve(1)->begin;
    }

    inline std::optional<Range> reserveRange(std::size_t size) {
        return manager->reserve(size);
    }

    inline bool free(Range range) { return manager->free(range); }

    inline bool free(std::size_t index) { return manager->free(index); }

    inline std::shared_ptr<Range> makeIndexGuard() {
        return PoolManager::makeRangeGuard(1, manager);
    }

    inline std::shared_ptr<Range> makeRangeGuard(std::size_t size) {
        return PoolManager::makeRangeGuard(size, manager);
    }

private:
    std::shared_ptr<PoolManager> manager;
};

} // namespace gnev::gl