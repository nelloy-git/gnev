#include <deque>

#include "gl/buffer/IBufferAllocator.hpp"

namespace gnev::gl {

class BufferAllocatorFragmented : public IBufferAllocator {
public:
    BufferAllocatorFragmented(std::unique_ptr<Buffer>&& buffer);
    ~BufferAllocatorFragmented() = default;

    BufferMemoryBlock* allocate(unsigned size) override;
    void deallocate(const BufferMemoryBlock*) override;

    void defragment();

private:
    struct FreeMemoryBlock {
        unsigned offset;
        unsigned size;
    };

    std::unique_ptr<Buffer> buffer;
    std::deque<FreeMemoryBlock> free_blocks;
    std::deque<std::unique_ptr<BufferMemoryBlock>> used_blocks;

    FreeMemoryBlock popFree(unsigned size);
    void pushFree(FreeMemoryBlock block);

    void eraseUsed(const BufferMemoryBlock* block);
    void pushUsed(const BufferMemoryBlock* block);  
};

} // namespace gnev::gl
