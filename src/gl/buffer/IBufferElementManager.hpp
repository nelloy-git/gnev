#pragma once

#include <functional>
#include <memory>
#include <optional>

#include "util/Export.hpp"

namespace gnev::gl {

class IBufferElementManager;

struct BufferElementBlock {
    unsigned first_index;
    unsigned count;
    const std::weak_ptr<IBufferElementManager> manager;

    bool operator==(const BufferElementBlock&) const = default;
};

class EXPORT IBufferElementManager {
public:
    using FreeCallback = std::function<
        void(const IBufferElementManager&, unsigned first_index, unsigned count)>;
    using MoveCallback = std::function<void(const IBufferElementManager&,
                                            unsigned src_first_index,
                                            unsigned dst_first_index,
                                            unsigned count)>;

    virtual ~IBufferElementManager(){};

    virtual std::shared_ptr<BufferElementBlock> reserve(unsigned size) = 0;
    virtual void optimize() = 0;

    virtual bool isFree(unsigned offset, unsigned size) const = 0;
    virtual bool isUsed(unsigned offset, unsigned size) const = 0;

    virtual unsigned getCapacity() const = 0;
    virtual unsigned getUsedCapacity() const = 0;
    virtual unsigned getFreeCapacity() const = 0;

    virtual void setFreeCallback(const std::optional<FreeCallback>& cb) = 0;
    virtual void setMoveCallback(const std::optional<MoveCallback>& cb) = 0;
};

} // namespace gnev::gl