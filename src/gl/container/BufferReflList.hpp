#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include "gl/container/BufferReflAccessor.hpp"
#include "gl/container/BufferReflArray.hpp"
#include "util/Export.hpp"
#include "util/RangeManager.hpp"

namespace gnev::gl {

template <IsReflStruct T>
class EXPORT BufferReflList {
public:
    BufferReflList(std::unique_ptr<IBufferRawAccessor>&& accessor_)
        : manager{std::make_shared<RangeManager>(static_cast<unsigned>(accessor_
                                                                           ->getBuffer()
                                                                           .getSize() /
                                                                       sizeof(T)))}
        , accessor{std::move(accessor_)} {}

    virtual ~BufferReflList() = default;

    std::optional<RangeManager::Range> reserveRange(unsigned size) {
        return manager->reserveRange(size);
    }

    bool freeRange(RangeManager::Range range) { return manager->freeRange(range); }

    std::shared_ptr<RangeManager::Range> makeRangeGuard() {
        return RangeManager::makeRangeGuard(manager);
    }

    std::vector<gl::BufferReflAccessor<T>> at(RangeManager::Range r) {
        std::vector<gl::BufferReflAccessor<T>> list;
        for (unsigned i = 0; i < r.size; ++i) {
            list.emplace_back(accessor, static_cast<unsigned>((r.begin + i) * sizeof(T)));
        }
        return list;
    }

    std::vector<const gl::BufferReflAccessor<T>> at(RangeManager::Range r) const {
        std::vector<const gl::BufferReflAccessor<T>> list;
        for (unsigned i = 0; i < r.size; ++i) {
            list.emplace_back(accessor, static_cast<unsigned>((r.begin + i) * sizeof(T)));
        }
        return list;
    }

private:
    std::shared_ptr<RangeManager> manager;
    std::shared_ptr<IBufferRawAccessor> accessor;
};

} // namespace gnev::gl