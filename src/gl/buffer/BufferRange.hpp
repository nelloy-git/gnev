#pragma once

#include "gl/buffer/BufferRangeControlBlock.hpp"

namespace gnev::gl {

class BufferRange {
    friend class BufferRangeAllocator;

public:
    inline unsigned getOffset() const { return data->pos.offset; };

    inline unsigned end() const { return data->pos.offset + data->pos.size; };

    inline unsigned size() const { return data->pos.size; };

private:
    BufferRange(const std::shared_ptr<BufferRangeData>& data)
        : data{data} {}

    std::shared_ptr<BufferRangeData> data;
};

} // namespace gnev::gl