#include "gl/buffer/BufferAllocator.hpp"

#include "util/InstanceLogger.hpp"

namespace gnev::gl {

using Mem = BufferAllocator::Mem;

BufferAllocator::BufferAllocator(unsigned capacity)
    : capacity{capacity}
    , free_size{capacity}
    , used_size{0}
    , used_greatest{0}
    , free_list{{.offset = 0, .size = capacity}}
    , used_list{}
    , free_callback{[](Mem) {}}
    , move_callback{[](Mem, Mem) {}} {}

Mem BufferAllocator::alloc(unsigned size) {
    auto alloc_mem = popFree(size);
    if (alloc_mem.size > 0) {
        pushUsed(alloc_mem);
    }
    return alloc_mem;
}

void BufferAllocator::free(Mem mem) {
    popUsed(mem);
    pushFree(mem);
    free_callback(mem);
}

void BufferAllocator::defragment() {
    unsigned total_used = 0;
    for (auto& mem : used_list) {
        total_used += mem.size;

        // Already in right place
        if (mem.offset == total_used) {
            continue;
        }

        Mem src_mem = mem;
        mem.offset = total_used;

        move_callback(src_mem, mem);
    }
    used_greatest = total_used;
}

unsigned BufferAllocator::getCapacity() const { return capacity; }

unsigned BufferAllocator::getFreeCapacity() const { return free_size; }

unsigned BufferAllocator::getUsedCapacity() const { return used_size; }

unsigned BufferAllocator::getGreatestUsed() const { return used_greatest; }

void BufferAllocator::setFreeCallback(const FreeCallback& cb) { free_callback = cb; }

void BufferAllocator::setMoveCallback(const MoveCallback& cb) { move_callback = cb; }

Mem BufferAllocator::popFree(unsigned size) {
    if (free_list.empty()) {
        InstanceLogger{}.Log<ERROR, "No free memory left">(size);
        return {.offset = 0, .size = 0};
    }

    auto cmp = [size](Mem free_mem) { return free_mem.size >= size; };
    auto found = std::find_if(free_list.begin(), free_list.end(), cmp);

    if (found == free_list.end()) {
        InstanceLogger{}.Log<ERROR, "Can not alloc mem of size {}">(size);
        return {.offset = 0, .size = 0};
    }

    Mem mem{.offset = found->offset, .size = size};
    if (found->size == size) {
        free_list.erase(found);
    } else {
        found->offset += size;
        found->size -= size;
    }
    free_size -= size;

    return mem;
}

void BufferAllocator::pushFree(Mem mem) {
    free_size += mem.size;

    if (free_list.empty()) {
        free_list.push_back(mem);
        return;
    }

    static constexpr auto cmp = [](Mem mem_1, Mem mem_2) {
        return mem_1.offset < mem_2.offset;
    };
    auto found = std::lower_bound(free_list.begin(), free_list.end(), mem, cmp);

    if (found == free_list.begin()) {
        // Insert before first free
        if (mem.offset + mem.size == found->offset) {
            // Merge
            found->offset = mem.offset;
        } else {
            free_list.insert(found, mem);
        }
    } else if (found == free_list.end()) {
        // Insert at the end of list
        auto& back = free_list.back();
        if (back.offset + back.size == mem.offset) {
            back.size += mem.size;
        } else {
            free_list.insert(found, mem);
        }
    } else {
        // Insert at middle of list
        auto prev = found - 1;
        if (prev->offset + prev->size == mem.offset) {
            // Merge to previous
            prev->size += mem.size;
            // Try merge increased prev with found one
            if (prev->offset + prev->size == found->offset) {
                prev->size += found->size;
                free_list.erase(found);
            }
        } else if (mem.offset + mem.size == found->offset) {
            found->offset = mem.offset;
        } else {
            free_list.insert(found, mem);
        }
    }
}

void BufferAllocator::pushUsed(Mem mem) {
    static constexpr auto cmp = [](Mem mem_1, Mem mem_2) {
        return mem_1.offset < mem_2.offset;
    };
    auto found = std::lower_bound(used_list.begin(), used_list.end(), mem, cmp);
    used_list.insert(found, mem);
    used_size += mem.size;
    used_greatest = std::max(used_greatest, mem.offset + mem.size);
}

void BufferAllocator::popUsed(Mem mem) {
    static constexpr auto cmp = [](Mem mem_1, Mem mem_2) {
        return mem_1.offset < mem_2.offset;
    };
    auto found = std::lower_bound(used_list.begin(), used_list.end(), mem, cmp);
    used_list.erase(found);
    used_size -= mem.size;
}

} // namespace gnev::gl