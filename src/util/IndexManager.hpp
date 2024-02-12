#pragma once

#include <deque>
#include <functional>
#include <memory>

#include "util/Export.hpp"
#include "util/Ref.hpp"
#include "util/WeakRef.hpp"

namespace gnev {

class IndexManager;

struct IndexGroup {
    unsigned first;
    unsigned count;

    bool operator==(const IndexGroup&) const = default;
};

class EXPORT IndexManager final : public std::enable_shared_from_this<IndexManager> {
public:
    using FreeCallback = std::function<void(const IndexGroup&)>;
    using MoveCallback =
        std::function<void(const IndexGroup& dst, const IndexGroup& src)>;

    static Ref<IndexManager> Make(unsigned capacity);

    std::optional<Ref<IndexGroup>> reserve(unsigned count,
                                           const FreeCallback& deleter_callback);
    void optimize(const MoveCallback& move_callback);

    bool isFree(unsigned index) const;

private:
    IndexManager(unsigned capacity);

    std::deque<IndexGroup> free;
    std::deque<WeakRef<IndexGroup>> used;

    void insertFree(IndexGroup& group);
    std::optional<Ref<IndexGroup>>
    eraseFree(unsigned count,
              const std::function<void(const IndexGroup&)>& deleter_callback);

    void insertUsed(const Ref<IndexGroup>& group);
    void eraseUsed(const IndexGroup& group);

    Ref<IndexGroup>
    makeIndexGroup(unsigned first,
                   unsigned count,
                   const std::function<void(const IndexGroup&)>& deleter_callback);
};

} // namespace gnev