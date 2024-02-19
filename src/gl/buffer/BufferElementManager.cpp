#include "gl/buffer/BufferElementManager.hpp"

#include "util/InstanceLogger.hpp"

namespace gnev::gl {

std::shared_ptr<BufferElementManager> BufferElementManager::Make(unsigned capacity) {
    auto ptr = std::shared_ptr<BufferElementManager>(new BufferElementManager(capacity));
    return ptr;
}

BufferElementManager::BufferElementManager(unsigned capacity)
    : capacity{capacity}
    , free{capacity}
    , used{0} {}

BufferElementManager::~BufferElementManager() {
    // TODO free memory
}

std::shared_ptr<BufferElementBlock> BufferElementManager::reserve(unsigned size) {
    auto free_block = popFree(size);
    if (not free_block.has_value()) {
        return nullptr;
    }

    auto reserved_block = MakeElementBlock(free_block.value());
    pushUsed(reserved_block);

    return reserved_block;
}

void BufferElementManager::optimize() {
    unsigned total_used = 0;
    for (auto& used_block : used) {
        if (used_block.first == total_used) {
            total_used += used_block.count;
            continue;
        }

        if (move_callback.has_value()) {
            move_callback.value()(*this, used_block.first, total_used, used_block.count);
        }

        used_block.first = total_used;
        auto mem_block = used_block.elemBlock.lock();
        mem_block->first_index = total_used;

        total_used += used_block.count;
    }
}

bool BufferElementManager::isFree(unsigned first, unsigned count) const {
    auto cmp = [first, count](const FreeBlock& block) {
        return block.first <= first and block.count <= count;
    };
    return std::find_if(free.begin(), free.end(), cmp) != free.end();
}

bool BufferElementManager::isUsed(unsigned first, unsigned count) const {
    return not isFree(first, count);
}

unsigned BufferElementManager::getCapacity() const { return capacity; }

unsigned BufferElementManager::getUsedCapacity() const { return used_size; }

unsigned BufferElementManager::getFreeCapacity() const { return free_size; }

void BufferElementManager::setFreeCallback(const std::optional<FreeCallback>& cb) {
    free_callback = cb;
}

void BufferElementManager::setMoveCallback(const std::optional<MoveCallback>& cb) {
    move_callback = cb;
}

void BufferElementManager::pushFree(const FreeBlock& block) {
    if (free.empty()) {
        free.push_back(block);
        free_size += block.count;
        return;
    }

    static constexpr auto cmp = [](const FreeBlock& block_1, const FreeBlock& block_2) {
        return block_1.first < block_2.first;
    };
    auto iter = std::lower_bound(free.begin(), free.end(), block, cmp);

    if (iter != free.begin() and iter != free.end()) {
        auto prev = iter - 1;

        if (prev->first + prev->count == block.first) {
            // Try merge to previous block
            prev->count += block.count;
            if (prev->first + prev->count == iter->first) {
                // Try merge previous and found block
                prev->count += iter->count;
                free.erase(iter);
            }
        } else if (block.first + block.count == iter->first) {
            // Try merge to found block
            iter->first = block.first;
        } else {
            // Merge fail
            free.insert(iter, block);
        }
    } else if (iter == free.begin()) {
        // Try merge to found block
        if (block.first + block.count == iter->first) {
            iter->first = block.first;
        } else {
            free.insert(iter, block);
        }
    } else if (iter == free.end()) {
        auto prev = iter - 1;
        // Try merge to previous block
        if (prev->first + prev->count == block.first) {
            prev->count += block.count;
        } else {
            free.insert(iter, block);
        }
    } else {
        throw std::runtime_error("Unexpected position");
    }

    free_size += block.count;
}

std::optional<BufferElementManager::FreeBlock>
BufferElementManager::popFree(unsigned size) {
    if (free.empty()) {
        InstanceLogger{}.Log<ERROR, "No free memory blocks left">(size);
        return std::nullopt;
    }

    auto cmp = [size](const FreeBlock& free_block) { return free_block.count >= size; };
    auto source_iter = std::find_if(free.begin(), free.end(), cmp);

    if (source_iter == free.end()) {
        InstanceLogger{}.Log<ERROR, "Can not reserve block of size {}">(size);
        return std::nullopt;
    }

    FreeBlock reserved_block{.first = source_iter->first, .count = size};
    source_iter->first += size;
    source_iter->count -= size;

    if (source_iter->count == 0) {
        free.erase(source_iter);
    }

    free_size -= size;
    return reserved_block;
}

void BufferElementManager::pushUsed(const std::shared_ptr<BufferElementBlock>& block) {
    UsedBlock used_block = {.first = block->first_index,
                            .count = block->count,
                            .elemBlock = block};

    static constexpr auto cmp = [](const UsedBlock& block_1, const UsedBlock& block_2) {
        return block_1.first < block_2.first;
    };
    auto iter = std::lower_bound(used.begin(), used.end(), used_block, cmp);
    used.insert(iter, used_block);
    used_size += used_block.count;
}

void BufferElementManager::popWeakUsed() {
    auto cmp = [](const UsedBlock& block) { return block.elemBlock.expired(); };
    auto iter = std::find_if(used.begin(), used.end(), cmp);
    used_size -= iter->count;
    used.erase(iter);
}

std::shared_ptr<BufferElementBlock>
BufferElementManager::MakeElementBlock(const FreeBlock& free_block) {
    static constexpr auto deleter = [](BufferElementBlock* block) {
        auto imanager = block->manager.lock();
        if (imanager) {
            auto manager = std::dynamic_pointer_cast<BufferElementManager>(imanager);
            if (manager) {
                manager->pushFree({.first = block->first_index, .count = block->count});
                manager->popWeakUsed();
                if (manager->free_callback.has_value()) {
                    manager->free_callback.value()(*manager, block->first_index, block->count);
                }
            }
        }
        delete block;
    };
    return std::shared_ptr<
        BufferElementBlock>(new BufferElementBlock{.first_index = free_block.first,
                                                   .count = free_block.count,
                                                   .manager = this->weak_from_this()},
                            deleter);
}

} // namespace gnev::gl