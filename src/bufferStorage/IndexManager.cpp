#include "bufferStorage/IndexManager.hpp"

#include <algorithm>

namespace gnev::gl {

IndexManager::IndexManager(unsigned capacity, unsigned first)
    : first(first)
    , last(first + capacity)
    , current_max(first) {}

std::optional<unsigned> IndexManager::reserveIndex() {
    if (unused.empty()) {
        if (current_max == last) {
            return std::nullopt;
        }
        return current_max++;
    }

    unsigned index = unused.front();
    unused.pop_front();
    return index;
}

bool IndexManager::freeIndex(unsigned index) {
    // out of range
    if (index < first or index >= last) {
        return false;
    }

    // never used
    if (index >= current_max) {
        return false;
    }

    // insert to empty list
    if (unused.empty()) {
        unused.emplace_back(index);
        return true;
    }

    auto found = std::lower_bound(unused.begin(), unused.end(), index);
    // already freed
    if (*found == index) {
        return false;
    }
    unused.insert(found, index);
    return true;
}

bool IndexManager::isUsed(unsigned index) const {
    if (index >= current_max) {
        return false;
    }
    auto found = std::lower_bound(unused.begin(), unused.end(), index);
    return *found != index;
}

unsigned IndexManager::getCapacity() const { return last - first; }

unsigned IndexManager::countFree() const { return last - current_max + unused.size(); }

unsigned IndexManager::countUsed() const { return current_max - (first + unused.size()); }

unsigned IndexManager::getMaxUsed() const { return current_max; }

} // namespace gnev::gl