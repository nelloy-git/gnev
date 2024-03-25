#include "util/IndexStorage.hpp"

#include <algorithm>

namespace gnev {

using Index = IndexManager::Index;

IndexManager::IndexManager(unsigned int capacity, Index first)
    : first(first)
    , last(first + capacity)
    , current_max(first) {}

std::optional<Index> IndexManager::reserveIndex() {
    std::lock_guard lg{m};

    if (unused.empty()) {
        if (current_max == last) {
            return std::nullopt;
        }
        return current_max++;
    } else {
        Index index = unused.front();
        unused.pop_front();
        return index;
    }
}

bool IndexManager::freeIndex(Index index) {
    std::lock_guard lg{m};

    // out of range
    if (index < first or index >= last) {
        return false;
    }

    if (index >= current_max) {
        return false;
    }

    auto iter = std::find(unused.begin(), unused.end(), index);
    if (iter != unused.end()) {
        return false;
    }

    unused.emplace_front(index);
    return true;
}

bool IndexManager::isUsed(Index index) const {
    std::lock_guard lg{m};
    if (index >= current_max) {
        return false;
    }
    auto iter = std::find(unused.begin(), unused.end(), index);
    return iter == unused.end();
}

unsigned int IndexManager::countFree() const {
    std::lock_guard lg{m};
    return last - current_max + unused.size();
}

unsigned int IndexManager::countUsed() const {
    std::lock_guard lg{m};
    return current_max - (first + unused.size());
}

unsigned int IndexManager::getCapacity() const { return last - first; }

unsigned int IndexManager::getMaxUsed() const {
    std::lock_guard lg{m};
    return current_max;
}

} // namespace gnev