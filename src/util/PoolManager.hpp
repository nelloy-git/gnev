#pragma once

#include <deque>
#include <memory>
#include <optional>

#include "util/Export.hpp"

namespace gnev {

class EXPORT PoolManager {
public:
    struct Range {
        const std::size_t begin;
        const std::size_t size;
    };

    // [first, first + capacity)
    PoolManager(std::size_t capacity);
    virtual ~PoolManager() = default;

    static std::shared_ptr<Range>
    makeRangeGuard(std::size_t size, const std::shared_ptr<PoolManager>& index_manager);

    // returns pos of first element in range
    std::optional<Range> reserve(std::size_t size);
    bool free(Range range);
    bool free(std::size_t index);

    // bool isInUse(Range range) const;
    // bool isInUse(std::size_t index) const;
    
    std::size_t countFree() const;
    std::size_t countUsed() const;
    std::size_t getCapacity() const;
    std::size_t getMaxUsed() const;

private:
    struct InternalRange {
        InternalRange(Range r)
            : begin{r.begin}
            , size{r.size} {}

        std::size_t begin;
        std::size_t size;
    };

    const std::size_t capacity;
    std::deque<InternalRange> free_ranges;

    void insertFreeRange(Range range);
};

} // namespace gnev