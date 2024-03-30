#include "util/RangeManager.hpp"

#include <algorithm>
#include <optional>

namespace gnev {

using Range = RangeManager::Range;

RangeManager::RangeManager(unsigned capacity)
    : free_ranges{Range{.begin = 0, .size = capacity}} {}

std::optional<Range> RangeManager::reserveRange(unsigned size) {
    auto cmp = [size](Range free_range) { return free_range.size >= size; };
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

bool RangeManager::freeRange(Range range) {
    insertFreeRange(range);
}

void RangeManager::insertFreeRange(Range range){
    static constexpr auto cmp =
        [](Range free_range, Range search_for) {
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

} // namespace gnev