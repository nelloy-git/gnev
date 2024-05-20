#include "gl/container/PoolController.hpp"

#include <algorithm>
#include <iterator>
#include <string>

#include "util/Log.hpp"

namespace gnev::gl {

std::string format_as(Range value) {
    if (value.size > 1) {
        return "[" + std::to_string(value.begin) + ", " +
               std::to_string(value.begin + value.size - 1) + "]";
    } else if (value.size == 1) {
        return "[" + std::to_string(value.begin) + "]";
    } else {
        return "[-]";
    }
}

PoolController::PoolController(unsigned size)
    : used(size, false) {}

unsigned PoolController::capacity() const { return used.size(); }

unsigned PoolController::size() const {
    return used.size() -
           std::distance(used.rbegin(), std::find(used.rbegin(), used.rend(), true));
}

bool PoolController::isInUse(unsigned pos) const { return used.at(pos); }

bool PoolController::isInUse(Range range) const {
    auto block_start = used.begin() + range.begin;
    auto block_end = block_start + range.size;
    return std::find(block_start, block_end, true) != block_end;
}

void PoolController::reserve(unsigned size) { used.resize(size, false); }

Range PoolController::pull(unsigned size) {
    if (size == 0) {
        GNEV_LOG(WARNING, "Trying pull zero sized range");
        return Range{.begin = 0, .size = 0};
    }

    auto block_start = used.begin();
    auto block_end = used.begin();
    Range result{.begin = 0, .size = 0};
    while (block_start != used.end()) {
        block_start = std::find(block_end, used.end(), false);
        block_end = std::find(block_start, used.end(), true);

        if (std::distance(block_start, block_end) >= size) {
            std::fill(block_start, block_start + size, true);
            result = {.begin =
                          static_cast<unsigned>(std::distance(used.begin(), block_start)),
                      .size = size};
            break;
        }
    }
    return result;
}

void PoolController::push(Range range) {
    auto block_start = used.begin() + range.begin;
    auto block_end = block_start + range.size;
    std::fill(block_start, block_end, false);
}

} // namespace gnev::gl