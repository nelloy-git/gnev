#pragma once

#include <vector>
#include <string>

#include "util/Export.hpp"

namespace gnev::gl {

struct Range {
    unsigned begin;
    unsigned size;
};

std::string EXPORT format_as(Range value);

class EXPORT PoolController {
public:
    PoolController(unsigned size);

    unsigned capacity() const;
    unsigned size() const;

    bool isInUse(unsigned pos) const;
    bool isInUse(Range range) const;

    void reserve(unsigned size);
    void shrinkToFit();

    Range pull(unsigned size);
    void push(Range range);

private:
    std::vector<bool> used;
};

} // namespace gnev::gl