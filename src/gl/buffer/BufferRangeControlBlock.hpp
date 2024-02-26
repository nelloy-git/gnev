#pragma once

#include <deque>
#include <functional>
#include <memory>

namespace gnev::gl {

struct BufferRangePos {
    unsigned offset;
    unsigned size;
};

class BufferRangeControlBlock;

struct BufferRangeData {
    BufferRangePos pos;
    std::weak_ptr<BufferRangeControlBlock> weak_control;
};

class BufferRangeControlBlock {
public:
    using FreeCallback = std::function<void(BufferRangePos)>;
    using MoveCallback = std::function<void(BufferRangePos src, BufferRangePos dst)>;

    BufferRangeControlBlock(unsigned capacity);

    unsigned capacity;
    unsigned free_size;
    unsigned used_size;

    std::deque<BufferRangePos> free_list;
    std::deque<std::pair<BufferRangePos, std::weak_ptr<BufferRangeData>>> used_list;

    FreeCallback free_callback;
    MoveCallback move_callback;
    
    BufferRangePos popFree(unsigned size);
    void pushFree(BufferRangePos pos);

    void popUsed(BufferRangePos pos);
    void pushUsed(BufferRangePos pos, std::weak_ptr<BufferRangeData> data);
};

} // namespace gnev::gl