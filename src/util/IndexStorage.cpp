#include "util/IndexStorage.hpp"

#include <algorithm>

namespace gnev {

using Index = IndexStorage::Index;

IndexStorage::IndexStorage(unsigned int capacity, Index first)
    : first(first)
    , last(first + capacity)
    , current(first) {}

std::optional<Index> IndexStorage::useIndex() {
    std::lock_guard lg{m};

    if (unused.empty()) {
        if (current == last) {
            return std::nullopt;
        }
        return current++;
    } else {
        Index index = unused.front();
        unused.pop_front();
        return index;
    }
}

bool IndexStorage::freeIndex(Index index) {
    std::lock_guard lg{m};
    
    // out of range
    if (index < first or index >= last) {
        return false;
    }

    if (index >= current) {
        return false;
    }

    auto iter = std::find(unused.begin(), unused.end(), index);
    if (iter != unused.end()) {
        return false;
    }

    unused.emplace_front(index);
    return true;
}

bool IndexStorage::isUsed(Index index) const {
    std::lock_guard lg{m};
    if (index >= current) {
        return false;
    }
    auto iter = std::find(unused.begin(), unused.end(), index);
    return iter == unused.end();
}

unsigned int IndexStorage::countFree() const {
    std::lock_guard lg{m};
    return last - current + unused.size();
}

} // namespace gnev