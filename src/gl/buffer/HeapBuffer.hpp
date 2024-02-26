#pragma once

#include <concepts>

#include "gl/buffer/BufferRangeAllocator.hpp"
#include "gl/buffer/IBufferAccessor.hpp"
#include "util/Export.hpp"

namespace gnev::gl {

template <typename T>
concept IsHeapBufferData = (not std::is_pointer_v<T> and not std::is_reference_v<T> and
                            std::is_trivially_copyable_v<T>);

template <IsHeapBufferData V>
class HeapBufferRange;

template <IsHeapBufferData T>
class EXPORT HeapBuffer {
    template <IsHeapBufferData V>
    friend class HeapBufferRange;

public:
    using FreeCallback = std::function<void(IBufferAccessor&, BufferRangePos)>;
    using MoveCallback =
        std::function<void(IBufferAccessor&, BufferRangePos src, BufferRangePos dst)>;

    HeapBuffer(std::unique_ptr<IBufferAccessor>&& accessor)
        : accessor{std::move(accessor)}
        , allocator{accessor->getBuffer().getSize() / sizeof(T)} {}

    std::optional<HeapBufferRange<T>> allocate(unsigned size) {
        auto range = allocator.allocate(size);
        if (not range.has_value()){
            return std::nullopt;
        }
        return HeapBufferRange<T>{accessor, range};
    }

    void setFreeCallback(const FreeCallback& callback) {
        auto wrapper = [this, callback](BufferRangePos pos) {
            callback(*this->accessor, pos);
        };
        allocator.setFreeCallback(wrapper);
    }

    void setMoveCallback(const MoveCallback& callback) {
        auto wrapper = [this, callback](BufferRangePos src, BufferRangePos dst) {
            callback(*this->accessor, src, dst);
        };
        allocator.setMoveCallback(wrapper);
    }

private:
    std::shared_ptr<IBufferAccessor> accessor;
    BufferRangeAllocator allocator;
};

} // namespace gnev::gl