#pragma once

#include <deque>
#include <functional>
#include <memory>
#include <optional>

#include "gl/buffer/BufferRange.hpp"
#include "gl/buffer/BufferRangeControlBlock.hpp"

namespace gnev::gl {

class BufferRangeAllocator {
public:
    using MoveCallback = BufferRangeControlBlock::MoveCallback;
    using FreeCallback = BufferRangeControlBlock::FreeCallback;

    BufferRangeAllocator(unsigned capacity);

    std::optional<BufferRange> allocate(unsigned size);
    void defragment();

    unsigned getCapacity() const;
    unsigned getUsedCapacity() const;
    unsigned getFreeCapacity() const;
    unsigned getMaxUsedCapacity() const;

    void setFreeCallback(const BufferRangeControlBlock::FreeCallback& cb);
    void setMoveCallback(const BufferRangeControlBlock::MoveCallback& cb);

private:
    std::shared_ptr<BufferRangeControlBlock> control_block;
};

} // namespace gnev::gl