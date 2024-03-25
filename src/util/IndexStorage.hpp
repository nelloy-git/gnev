#pragma once

#include <list>
#include <mutex>
#include <optional>

#include "util/Export.hpp"

namespace gnev {

class EXPORT IndexManager {
public:
    using Index = unsigned int;

    // [first, first + capacity)
    IndexManager(unsigned int capacity, Index first = 0);
    virtual ~IndexManager() = default;

    std::optional<Index> reserveIndex();
    bool freeIndex(Index index);
    bool isUsed(Index index) const;
    unsigned int countFree() const;
    unsigned int countUsed() const;
    unsigned int getCapacity() const;
    unsigned int getMaxUsed() const;

private:
    mutable std::mutex m;
    const unsigned int first;
    const unsigned int last;

    Index current_max;
    std::list<Index> unused;
};

} // namespace gnev