#pragma once

#include <memory>
#include <optional>

#include "util/Ref.hpp"

namespace gnev {

template <typename T>
class WeakRef {
    template<typename V>
    friend class WeakRef;

public:
    WeakRef(const Ref<T>& ref)
        : ptr(ref.ptr) {}

    WeakRef(Ref<T>&& ref)
        : ptr(std::move(ref.ptr)) {}

    template <typename V>
    WeakRef(const WeakRef<V>& other)
        : ptr(other.ptr) {}

    template <typename V>
    WeakRef(WeakRef<V>&& other)
        : ptr(std::move(other.ptr)) {}

    std::optional<Ref<T>> lock() const {
        auto shared_ptr = ptr.lock();
        return shared_ptr ? std::optional<Ref<T>>(shared_ptr) : std::nullopt;
    };

private:
    std::weak_ptr<T> ptr;
};

} // namespace gnev