#pragma once

#include <deque>
#include <memory>
#include <optional>

#include "util/Export.hpp"

namespace gnev {

class EXPORT RangeManager {
public:
    struct Range {
        unsigned begin;
        unsigned size;
    };

    // [first, first + capacity)
    RangeManager(unsigned capacity);
    virtual ~RangeManager() = default;

    static std::shared_ptr<unsigned>
    makeRangeGuard(const std::shared_ptr<RangeManager>& index_manager);

    // returns pos of first element in range
    std::optional<Range> reserveRange(unsigned size);
    bool freeRange(Range range);
    unsigned int countFree() const;
    unsigned int countUsed() const;
    unsigned int getCapacity() const;
    unsigned int getMaxUsed() const;

private:
    std::deque<Range> free_ranges;

    void insertFreeRange(Range range);
};

} // namespace gnev