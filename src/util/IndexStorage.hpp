#pragma once

#include <list>
#include <mutex>
#include <optional>

#include "util/Export.hpp"

namespace gnev {

class EXPORT IndexStorage {
public:
    using Index = unsigned int;

    // [first, first + capacity)
    IndexStorage(unsigned int capacity,
                 Index first = 0);
    virtual ~IndexStorage() = default;

    std::optional<Index> useIndex();
    bool freeIndex(Index index);
    bool isUsed(Index index) const;
    unsigned int countFree() const;
    unsigned int getCapacity() const;

private:
    mutable std::mutex m;
    const unsigned int first;
    const unsigned int last;

    Index current;
    std::list<Index> unused;
};

} // namespace gnev