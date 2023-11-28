#pragma once

#include <memory>
#include <optional>

#include "util/StrongRef.hpp"

namespace gnev {

template <typename T>
class WeakRef {
public:
    WeakRef(const StrongRef<T>& ref)
        : ptr(ref.ptr) {}

    std::optional<StrongRef<T>> lock() const {
        auto shared_ptr = ptr.lock();
        return shared_ptr ? std::optional<StrongRef<T>>(shared_ptr) : std::nullopt;
    };

private:
    std::weak_ptr<T> ptr;
};

} // namespace gnev