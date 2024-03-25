#pragma once

#include <list>
#include <memory>
#include <optional>

#include "util/Export.hpp"

namespace gnev::gl {

class EXPORT IndexManager {
public:
    // [first, first + capacity)
    IndexManager(unsigned capacity, unsigned first = 0);
    virtual ~IndexManager() = default;

    std::optional<unsigned> reserveIndex();
    bool freeIndex(unsigned index);
    bool isUsed(unsigned index) const;

    unsigned getCapacity() const;
    unsigned countFree() const;
    unsigned countUsed() const;
    unsigned getMaxUsed() const;

private:
    const unsigned first;
    const unsigned last;

    unsigned current_max;
    std::list<unsigned> unused;
};

} // namespace gnev::gl