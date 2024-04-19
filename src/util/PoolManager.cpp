#include "util/PoolManager.hpp"

#include <algorithm>
#include <cstddef>
#include <optional>

namespace gnev {

using Range = PoolManager::Range;

PoolManager::PoolManager(std::size_t capacity)
    : capacity{capacity}
    , free_ranges{Range{.begin = 0, .size = capacity}} {}

std::shared_ptr<Range>
PoolManager::makeRangeGuard(std::size_t size,
                            const std::shared_ptr<PoolManager>& range_manager) {
    auto range = range_manager->reserve(size);
    if (not range.has_value()) {
        return nullptr;
    }

    auto deleter = [weak_manager = std::weak_ptr<PoolManager>{range_manager}](Range* p) {
        auto index_manager = weak_manager.lock();
        if (index_manager) {
            index_manager->free(*p);
        }
        delete p;
    };
    return std::shared_ptr<Range>{new Range{range.value()}, deleter};
}

std::optional<Range> PoolManager::reserve(std::size_t size) {
    auto cmp = [size](InternalRange free_range) { return free_range.size >= size; };
    auto found = std::find_if(free_ranges.begin(), free_ranges.end(), cmp);

    if (found == free_ranges.end()) {
        return std::nullopt;
    }

    Range range = {.begin = found->begin, .size = size};
    found->begin += size;
    found->size -= size;

    if (found->size == 0) {
        free_ranges.erase(found);
    }

    return range;
}

bool PoolManager::free(Range range) {
    insertFreeRange(range);
    return true;
}

bool PoolManager::free(std::size_t index) {
    insertFreeRange(Range{.begin = index, .size = 1});
    return true;
}

void PoolManager::insertFreeRange(Range range) {
    static constexpr auto cmp = [](InternalRange free_range, Range search_for) {
        return free_range.begin < search_for.begin;
    };

    auto found = std::lower_bound(free_ranges.begin(), free_ranges.end(), range, cmp);

    if (found == free_ranges.begin()) {
        // Insert before first free
        if (range.begin + range.size == found->begin) {
            // Merge
            found->begin = range.begin;
        } else {
            free_ranges.insert(found, range);
        }
    } else if (found == free_ranges.end()) {
        // Insert at the end of list
        auto& back = free_ranges.back();
        if (back.begin + back.size == range.begin) {
            back.size += range.size;
        } else {
            free_ranges.insert(found, range);
        }
    } else {
        // Insert at middle of list
        auto prev = found - 1;
        if (prev->begin + prev->size == range.begin) {
            // Merge to previous
            prev->size += range.size;
            // Try merge increased prev with found one
            if (prev->begin + prev->size == found->begin) {
                prev->size += found->size;
                free_ranges.erase(found);
            }
        } else if (range.begin + range.size == found->begin) {
            found->begin = range.begin;
        } else {
            free_ranges.insert(found, range);
        }
    }
}

std::size_t PoolManager::countFree() const {
    std::size_t free = 0;
    for (auto& range : free_ranges) {
        free += range.size;
    }
    return free;
}

std::size_t PoolManager::countUsed() const { return capacity - countFree(); }

std::size_t PoolManager::getCapacity() const { return capacity; }

std::size_t PoolManager::getMaxUsed() const {
    if (free_ranges.size() == 0) {
        return capacity;
    }
    return free_ranges.back().begin + free_ranges.back().size;
}

} // namespace gnev