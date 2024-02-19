#pragma once

#include <deque>
#include <functional>

namespace gnev::gl {

class BufferAllocator {
public:
    struct Mem {
        unsigned offset;
        unsigned size;

        operator bool() const { return size != 0; }
    };

    using FreeCallback = std::function<void(Mem)>;
    using MoveCallback = std::function<void(Mem src, Mem dst)>;

    BufferAllocator(unsigned capacity);

    Mem alloc(unsigned size);
    void free(Mem mem);
    void defragment();

    unsigned getCapacity() const;
    unsigned getUsedCapacity() const;
    unsigned getFreeCapacity() const;
    unsigned getGreatestUsed() const;

    void setFreeCallback(const FreeCallback& cb);
    void setMoveCallback(const MoveCallback& cb);

private:
    unsigned capacity;
    unsigned free_size;
    unsigned used_size;
    unsigned used_greatest;

    std::deque<Mem> free_list;
    std::deque<Mem> used_list;

    FreeCallback free_callback;
    MoveCallback move_callback;

    Mem popFree(unsigned size);
    void pushFree(Mem mem);

    void popUsed(Mem mem);
    void pushUsed(Mem mem);
};

} // namespace gnev::gl