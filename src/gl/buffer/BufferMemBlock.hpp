#pragma once

#include <memory>

namespace gnev::gl {

class IBufferMemManagerControlBlock;

class BufferMemBlock {
public:
    struct Info {
        unsigned offset;
        unsigned size;
        void* user_data;
        std::weak_ptr<IBufferMemManagerControlBlock> weak_control_block;
    };

    BufferMemBlock(const Info& info_)
        : info{std::make_shared<Info>(info_)} {}

    inline unsigned getOffset() const { return info->offset; }

    inline unsigned getSize() const { return info->size; }

    inline void* getUserData() { return info->user_data; }

    inline const void* getUserData() const { return info->user_data; }

    inline std::weak_ptr<IBufferMemManagerControlBlock> getControlBlock() const {
        return info->weak_control_block;
    }

private:
    std::shared_ptr<Info> info;
};

} // namespace gnev::gl