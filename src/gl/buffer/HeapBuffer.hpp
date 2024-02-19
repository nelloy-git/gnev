#pragma once

#include <concepts>

#include "gl/buffer/BufferAllocator.hpp"
#include "gl/buffer/IBufferAccessor.hpp"
#include "util/Export.hpp"

namespace gnev::gl {
template <typename T>
concept IsHeapBufferData = (not std::is_pointer_v<T> and std::is_trivially_copyable_v<T>);

template <IsHeapBufferData T>
class EXPORT HeapBuffer {
public:
    using Element = T;
    using FreeCallback =
        std::function<void(const HeapBuffer&, const BufferElementBlock&)>;
    using MoveCallback = std::function<
        void(const HeapBuffer&, unsigned src, unsigned dst, unsigned count)>;

    static constexpr unsigned ElementSize = std::sizeof(T);

    HeapBuffer(std::unique_ptr<IBufferAccessor>&& accessor,
               std::unique_ptr<BufferAllocator>&& allocator) {}

private:
};
} // namespace gnev::gl