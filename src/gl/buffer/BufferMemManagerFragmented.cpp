// #include "gl/buffer/BufferMemManagerFragmented.hpp"

// #include "util/InstanceLogger.hpp"

// namespace gnev::gl {

// BufferMemManagerFragmented::BufferMemManagerFragmented(unsigned capacity)
//     : control_block{std::make_shared<BufferMemManagerControlBlock>(capacity)} {}

// std::optional<BufferMemBlock> BufferMemManagerFragmented::allocate(unsigned size) {
//     auto pos = control_block->popFree(size);
//     if (pos.size == 0) {
//         return std::nullopt;
//     }

//     std::shared_ptr<BufferMemBlockInfo> data{
//         new BufferMemBlockInfo(pos, control_block),
//         [](BufferMemBlockInfo* data) {
//             auto control_block = data->weak_control.lock();
//             if (control_block) {
//                 control_block->pushFree(data->pos);
//                 control_block->popUsed(data->pos);
//                 control_block->free_callback(data->pos);
//             }
//             delete data;
//         }};
//     control_block->pushUsed(pos, data);
//     return BufferMemBlock{data};
// }

// void BufferMemManagerFragmented::defragment() {
//     unsigned total_used = 0;
//     for (auto& iter : control_block->used_list) {
//         auto ptr = iter.second.lock();
//         // Drop if do not exist
//         if (!ptr) {
//             continue;
//         }

//         BufferMemBlockPos current_pos = iter.first;
//         // Already in right place
//         if (current_pos.offset == total_used) {
//             total_used += iter.first.size;
//             continue;
//         }

//         BufferMemBlockPos target_pos = {.offset = total_used, .size = current_pos.size};
//         control_block->move_callback(current_pos, target_pos);
//         iter.first = target_pos;
//         ptr->pos = target_pos;

//         total_used += iter.first.size;
//     }
// }

// unsigned BufferMemManagerFragmented::getCapacity() const {
//     return control_block->capacity;
// }

// unsigned BufferMemManagerFragmented::getFreeCapacity() const {
//     return control_block->free_size;
// }

// unsigned BufferMemManagerFragmented::getUsedCapacity() const {
//     return control_block->used_size;
// }

// unsigned BufferMemManagerFragmented::getMaxUsedCapacity() const {
//     return control_block->used_list.back().first.offset +
//            control_block->used_list.back().first.size;
// }

// void BufferMemManagerFragmented::setFreeCallback(const FreeCallback& cb) {
//     control_block->free_callback = cb;
// }

// void BufferMemManagerFragmented::setMoveCallback(const MoveCallback& cb) {
//     control_block->move_callback = cb;
// }

// } // namespace gnev::gl