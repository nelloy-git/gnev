#include "gl/buffer/BufferRangeAllocator.hpp"

#include "util/InstanceLogger.hpp"

namespace gnev::gl {

BufferRangeAllocator::BufferRangeAllocator(unsigned capacity)
    : control_block{std::make_shared<BufferRangeControlBlock>(capacity)} {}

std::optional<BufferRange> BufferRangeAllocator::allocate(unsigned size) {
    auto pos = control_block->popFree(size);
    if (pos.size == 0) {
        return std::nullopt;
    }

    std::shared_ptr<BufferRangeData> data{new BufferRangeData(pos, control_block),
                                          [](BufferRangeData* data) {
                                              auto control_block =
                                                  data->weak_control.lock();
                                              if (control_block) {
                                                  control_block->pushFree(data->pos);
                                                  control_block->popUsed(data->pos);
                                                  control_block->free_callback(data->pos);
                                              }
                                              delete data;
                                          }};
    control_block->pushUsed(pos, data);
    return BufferRange{data};
}

void BufferRangeAllocator::defragment() {
    unsigned total_used = 0;
    for (auto& iter : control_block->used_list) {
        auto ptr = iter.second.lock();
        // Drop if do not exist
        if (!ptr) {
            continue;
        }

        BufferRangePos current_pos = iter.first;
        // Already in right place
        if (current_pos.offset == total_used) {
            total_used += iter.first.size;
            continue;
        }

        BufferRangePos target_pos = {.offset = total_used, .size = current_pos.size};
        control_block->move_callback(current_pos, target_pos);
        iter.first = target_pos;
        ptr->pos = target_pos;

        total_used += iter.first.size;
    }
}

unsigned BufferRangeAllocator::getCapacity() const { return control_block->capacity; }

unsigned BufferRangeAllocator::getFreeCapacity() const {
    return control_block->free_size;
}

unsigned BufferRangeAllocator::getUsedCapacity() const {
    return control_block->used_size;
}

unsigned BufferRangeAllocator::getMaxUsedCapacity() const {
    return control_block->used_list.back().first.offset +
           control_block->used_list.back().first.size;
}

void BufferRangeAllocator::setFreeCallback(const FreeCallback& cb) {
    control_block->free_callback = cb;
}

void BufferRangeAllocator::setMoveCallback(const MoveCallback& cb) {
    control_block->move_callback = cb;
}

} // namespace gnev::gl