// #pragma once

// #include <deque>
// #include <functional>
// #include <memory>

// namespace gnev::gl {

// struct BufferMemBlockPos {
//     unsigned offset;
//     unsigned size;
// };

// class BufferMemManagerControlBlock;

// struct BufferMemBlockInfo {
//     BufferMemBlockPos pos;
//     std::weak_ptr<BufferMemManagerControlBlock> weak_control;
// };

// class BufferMemManagerControlBlock {
// public:
//     using FreeCallback = std::function<void(BufferMemBlockPos)>;
//     using MoveCallback =
//         std::function<void(BufferMemBlockPos src, BufferMemBlockPos dst)>;

//     BufferMemManagerControlBlock(unsigned capacity);

//     unsigned capacity;
//     unsigned free_size;
//     unsigned used_size;

//     std::deque<BufferMemBlockPos> free_list;
//     std::deque<std::pair<BufferMemBlockPos, std::weak_ptr<BufferMemBlockInfo>>> used_list;

//     FreeCallback free_callback;
//     MoveCallback move_callback;

//     BufferMemBlockPos popFree(unsigned size);
//     void pushFree(BufferMemBlockPos pos);

//     void popUsed(BufferMemBlockPos pos);
//     void pushUsed(BufferMemBlockPos pos, std::weak_ptr<BufferMemBlockInfo> data);
// };

// } // namespace gnev::gl