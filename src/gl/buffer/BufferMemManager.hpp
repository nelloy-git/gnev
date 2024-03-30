// #pragma once

// #include "gl/buffer/IBufferMemManager.hpp"
// #include "gl/buffer/IBufferMemManagerControlBlock.hpp"

// namespace gnev::gl {

// class BufferMemManager : public IBufferMemManager {
// public:
//     BufferMemManager(unsigned capacity);

//     std::optional<BufferMemBlock> allocate(unsigned size) override;

//     unsigned getCapacity() const;
//     unsigned getUsedSize() const;
//     unsigned getFreeSize() const;

// private:
//     std::shared_ptr<IBufferMemManagerControlBlock> control_block;

// };

// } // namespace gnev::gl