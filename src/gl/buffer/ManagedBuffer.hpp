#pragma once

#include <memory>

namespace gnev::gl {

class Buffer;
class IBufferMemManager;
class IBufferAccessor;

class ManagedBuffer {
public:
    ManagedBuffer();

private:
    std::unique_ptr<Buffer> buffer;
    std::unique_ptr<IBufferMemManager> manager;
    std::unique_ptr<IBufferAccessor> accessor;
};

} // namespace gnev::gl