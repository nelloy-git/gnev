#pragma once

#include <deque>
#include <memory>
#include <optional>

#include "util/Export.hpp"

namespace gnev {

class EXPORT RangeManager {
public:
    struct Range {
        const unsigned begin;
        const unsigned size;
    };

    // [first, first + capacity)
    RangeManager(unsigned capacity);
    virtual ~RangeManager() = default;

    static std::shared_ptr<Range>
    makeRangeGuard(unsigned size, const std::shared_ptr<RangeManager>& index_manager);

    // returns pos of first element in range
    std::optional<Range> reserveRange(unsigned size);
    bool freeRange(Range range);
    
    unsigned countFree() const;
    unsigned countUsed() const;
    unsigned getCapacity() const;
    unsigned getMaxUsed() const;

private:
    struct InternalRange {
        InternalRange(Range r)
            : begin{r.begin}
            , size{r.size} {}

        unsigned begin;
        unsigned size;
    };

    const unsigned capacity;
    std::deque<InternalRange> free_ranges;

    void insertFreeRange(Range range);
};

} // namespace gnev