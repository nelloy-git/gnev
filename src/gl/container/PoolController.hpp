#pragma once

#include <vector>
#include <string>

namespace gnev::gl {

struct Range {
    unsigned begin;
    unsigned size;
};

std::string format_as(Range value);

class PoolController {
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