// #pragma once

// #include <deque>
// #include <functional>
// #include <memory>
// #include <optional>

// #include "gl/buffer/BufferMemBlock.hpp"
// #include "gl/buffer/BufferMemManagerControlBlock.hpp"
// #include "gl/buffer/IBufferMemManager.hpp"

// namespace gnev::gl {

// class BufferMemManagerFragmented : public IBufferMemManager {
// public:
//     using MoveCallback = BufferMemManagerControlBlock::MoveCallback;
//     using FreeCallback = BufferMemManagerControlBlock::FreeCallback;

//     BufferMemManagerFragmented(unsigned capacity);

//     std::optional<BufferMemBlock> allocate(unsigned size) override;
//     void defragment();

//     unsigned getCapacity() const;
//     unsigned getUsedCapacity() const;
//     unsigned getFreeCapacity() const;
//     unsigned getMaxUsedCapacity() const;

//     void setFreeCallback(const BufferMemManagerControlBlock::FreeCallback& cb);
//     void setMoveCallback(const BufferMemManagerControlBlock::MoveCallback& cb);

// private:
//     std::shared_ptr<BufferMemManagerControlBlock> control_block;
// };

// } // namespace gnev::gl