#pragma once

#include <deque>

#include "gl/buffer/IBufferElementManager.hpp"

namespace gnev::gl {

class EXPORT BufferElementManager final
    : public IBufferElementManager
    , protected std::enable_shared_from_this<BufferElementManager> {
public:
    static std::shared_ptr<BufferElementManager> Make(unsigned capacity);
    ~BufferElementManager();

    std::shared_ptr<BufferElementBlock> reserve(unsigned size) override;
    void optimize() override;

    bool isFree(unsigned first, unsigned count) const override;
    bool isUsed(unsigned first, unsigned count) const override;

    unsigned getCapacity() const override;
    unsigned getUsedCapacity() const override;
    unsigned getFreeCapacity() const override;

    void setFreeCallback(const std::optional<FreeCallback>& cb) override;
    void setMoveCallback(const std::optional<MoveCallback>& cb) override;

private:
    BufferElementManager(unsigned capacity);

    struct FreeBlock {
        unsigned first;
        unsigned count;
    };

    struct UsedBlock {
        unsigned first;
        unsigned count;
        std::weak_ptr<BufferElementBlock> elemBlock;
    };

    std::optional<FreeCallback> free_callback;
    std::optional<MoveCallback> move_callback;

    std::deque<FreeBlock> free;
    std::deque<UsedBlock> used;

    unsigned capacity;
    unsigned free_size;
    unsigned used_size;

    void pushFree(const FreeBlock& block);
    std::optional<FreeBlock> popFree(unsigned size);

    void pushUsed(const std::shared_ptr<BufferElementBlock>& block);
    void popWeakUsed();

    std::shared_ptr<BufferElementBlock> MakeElementBlock(const FreeBlock& group);
};

} // namespace gnev::gl