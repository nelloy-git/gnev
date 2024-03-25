#include "gl/buffer/BufferMemManagerControlBlock.hpp"

#include "util/InstanceLogger.hpp"

namespace gnev::gl {

BufferMemManagerControlBlock::BufferMemManagerControlBlock(unsigned capacity)
    : capacity{capacity}
    , free_size{capacity}
    , used_size{0}
    , free_list{BufferMemBlockPos{.offset = 0, .size = capacity}}
    , used_list{}
    , free_callback{[](BufferMemBlockPos) {}}
    , move_callback{[](BufferMemBlockPos, BufferMemBlockPos) {}} {}

BufferMemBlockPos BufferMemManagerControlBlock::popFree(unsigned size) {
    if (free_list.empty()) {
        InstanceLogger{}.Log<ERROR, "No free memory left">(size);
        return {.offset = 0, .size = 0};
    }

    auto cmp = [size](BufferMemBlockPos free_mem) { return free_mem.size >= size; };
    auto found = std::find_if(free_list.begin(), free_list.end(), cmp);

    if (found == free_list.end()) {
        InstanceLogger{}.Log<ERROR, "Can not alloc mem of size {}">(size);
        return {.offset = 0, .size = 0};
    }

    BufferMemBlockPos mem{.offset = found->offset, .size = size};
    if (found->size == size) {
        free_list.erase(found);
    } else {
        found->offset += size;
        found->size -= size;
    }
    free_size -= size;

    return mem;
}

void BufferMemManagerControlBlock::pushFree(BufferMemBlockPos pos) {
    free_size += pos.size;

    if (free_list.empty()) {
        free_list.push_back(pos);
        return;
    }

    static constexpr auto cmp = [](BufferMemBlockPos pos_1, BufferMemBlockPos pos_2) {
        return pos_1.offset < pos_2.offset;
    };
    auto found = std::lower_bound(free_list.begin(), free_list.end(), pos, cmp);

    if (found == free_list.begin()) {
        // Insert before first free
        if (pos.offset + pos.size == found->offset) {
            // Merge
            found->offset = pos.offset;
        } else {
            free_list.insert(found, pos);
        }
    } else if (found == free_list.end()) {
        // Insert at the end of list
        auto& back = free_list.back();
        if (back.offset + back.size == pos.offset) {
            back.size += pos.size;
        } else {
            free_list.insert(found, pos);
        }
    } else {
        // Insert at middle of list
        auto prev = found - 1;
        if (prev->offset + prev->size == pos.offset) {
            // Merge to previous
            prev->size += pos.size;
            // Try merge increased prev with found one
            if (prev->offset + prev->size == found->offset) {
                prev->size += found->size;
                free_list.erase(found);
            }
        } else if (pos.offset + pos.size == found->offset) {
            found->offset = pos.offset;
        } else {
            free_list.insert(found, pos);
        }
    }
}

void BufferMemManagerControlBlock::popUsed(BufferMemBlockPos pos) {
    static constexpr auto cmp =
        [](std::pair<BufferMemBlockPos, std::weak_ptr<BufferMemBlockInfo>> element,
           BufferMemBlockPos search_for) {
            return element.first.offset < search_for.offset;
        };

    auto found = std::lower_bound(used_list.begin(), used_list.end(), pos, cmp);
    if (found->first.offset != pos.offset or found->first.size != pos.size) {
        InstanceLogger{}
            .Log<ERROR, "BufferMemPos(offset = {}, size = {}) is not found">(pos.offset,
                                                                             pos.size);
        return;
    }

    used_list.erase(found);
    used_size -= pos.size;
}

void BufferMemManagerControlBlock::pushUsed(BufferMemBlockPos pos,
                                            std::weak_ptr<BufferMemBlockInfo> data) {
    static constexpr auto cmp =
        [](std::pair<BufferMemBlockPos, std::weak_ptr<BufferMemBlockInfo>> element,
           BufferMemBlockPos search_for) {
            return element.first.offset < search_for.offset;
        };
    auto found = std::lower_bound(used_list.begin(), used_list.end(), pos, cmp);
    used_list.insert(found, std::make_pair(pos, data));
    used_size += pos.size;
}

} // namespace gnev::gl