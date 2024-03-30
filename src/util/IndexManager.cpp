#include "util/IndexManager.hpp"

#include <algorithm>
#include <memory>

namespace gnev {

using Index = IndexManager::Index;

IndexManager::IndexManager(unsigned int capacity, Index first)
    : first(first)
    , last(first + capacity)
    , current_max(first) {}

std::shared_ptr<unsigned>
IndexManager::makeIndexGuard(const std::shared_ptr<IndexManager>& index_manager) {
    auto index = index_manager->reserveIndex();
    if (not index.has_value()) {
        return nullptr;
    }

    auto deleter = [weak_manager = std::weak_ptr<IndexManager>{index_manager}](unsigned* p) {
        auto index_manager = weak_manager.lock();
        if (index_manager){
            index_manager->freeIndex(*p);
        }
        delete p;
    };
    return std::shared_ptr<unsigned>{new unsigned{index.value()}, deleter};
}

std::optional<Index> IndexManager::reserveIndex() {
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

bool IndexManager::isInUse(Index index) const {
    if (index >= current_max) {
        return false;
    }
    auto iter = std::find(unused.begin(), unused.end(), index);
    return iter == unused.end();
}

unsigned int IndexManager::countFree() const {
    return last - current_max + unused.size();
}

unsigned int IndexManager::countUsed() const {
    return current_max - (first + unused.size());
}

unsigned int IndexManager::getCapacity() const { return last - first; }

unsigned int IndexManager::getMaxUsed() const { return current_max; }

} // namespace gnev