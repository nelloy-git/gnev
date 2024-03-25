#pragma once

#include "bufferStorage/FixedSlot.hpp"
#include "bufferStorage/IBufferAccessor.hpp"
#include "bufferStorage/IndexManager.hpp"
#include "gl/Buffer.hpp"
#include "gl/ReflStruct.hpp"

namespace gnev::gl {

template <IsReflStruct T>
class FixedSlotStorage {
public:
    FixedSlotStorage(std::unique_ptr<Buffer>&& buffer_,
                     std::unique_ptr<IBufferAccessor>&& accessor)
        : buffer{std::move(buffer_)}
        , accessor{std::move(accessor)}
        , manager{std::make_shared<IndexManager>(buffer->getSize() / sizeof(T))} {
            accessor->bindBuffer(buffer.get());
        }

    FixedSlotStorage(const FixedSlotStorage&) = delete;
    FixedSlotStorage(FixedSlotStorage&&) = delete;

    ~FixedSlotStorage() { accessor->bindBuffer(nullptr); }

    std::unique_ptr<FixedSlot<T>> reserve() {
        auto index_opt = manager->reserveIndex();
        if (not index_opt.has_value()) {
            return nullptr;
        }
        return std::unique_ptr<FixedSlot<T>>(new FixedSlot<T>(index_opt.value(),
                                                              manager,
                                                              accessor));
    }

private:
    std::unique_ptr<gl::Buffer> buffer;
    std::shared_ptr<IBufferAccessor> accessor;
    std::shared_ptr<IndexManager> manager;
};

} // namespace gnev::gl