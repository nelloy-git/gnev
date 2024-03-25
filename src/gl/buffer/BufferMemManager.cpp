// #include "gl/buffer/BufferMemManager.hpp"

// #include <deque>

// namespace gnev::gl {

// class ControlBlock : public IBufferMemManagerControlBlock {
// public:
//     ControlBlock(unsigned capacity)
//         : capacity{capacity}
//         , free_size{capacity}
//         , used_size{0} {}

//     unsigned getCapacity() const override { return capacity; }

//     unsigned getFreeSize() const override { return free_size; }

//     unsigned getUsedSize() const override { return used_size; }

//     unsigned capacity;
//     unsigned free_size;
//     unsigned used_size;

//     struct FreeBlock {
//         unsigned offset;
//         unsigned size;
//     };

//     struct UsedBlock {
//         unsigned offset;
//         unsigned size;
//         std::weak_ptr<BufferMemBlock::Info> ptr;
//     };

//     std::deque<FreeBlock> free_blocks;
//     std::deque<UsedBlock> used_blocks;
// };

// BufferMemManager::BufferMemManager(unsigned capacity)
//     : control_block{std::make_shared<ControlBlock>(capacity)} {}

// } // namespace gnev::gl