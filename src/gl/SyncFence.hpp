#pragma once

#include <optional>

#include "Ctx.hpp"
#include "gl/enum/SyncStatus.hpp"

namespace gnev::gl {

class EXPORT SyncFence {
public:
    static constexpr GLuint64 default_wait_ns = 1000000000 / 240; // 1 frame at 240Hz

    SyncFence();
    virtual ~SyncFence() = default;

    SyncStatus wait(GLuint64 wait_ns = default_wait_ns);
    std::optional<SyncStatus> lastStatus() const;

private:
    GLsync sync;
    std::optional<SyncStatus> last_status;
};

} // namespace gnev::gl