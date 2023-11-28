#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>

namespace gnev {

template <typename T>
class StrongRef {
    template <typename V>
    friend class WeakRef;

public:
    template <typename... Args>
    static StrongRef<T> Make(Args&&... args)
        requires(std::constructible_from<T, Args...>)
    {
        auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
        return StrongRef<T>(ptr);
    }

    template <typename V>
    StrongRef(const std::shared_ptr<V>& ptr)
        : ptr(ptr) {
        if (not ptr) {
            throw std::logic_error("");
        }
    }

    StrongRef(const StrongRef&) = default;
    StrongRef(StrongRef&&) = default;
    StrongRef& operator=(const StrongRef&) = default;
    StrongRef& operator=(StrongRef&&) = default;

    ~StrongRef() {}

    operator std::shared_ptr<T>() const { return ptr; }

    T* operator->() { return ptr.get(); }

    const T* operator->() const { return ptr.get(); }

    bool operator==(const StrongRef<T>& other) const { return ptr == other.ptr; }

    operator const T&() const { return *ptr; }

    operator T&() { return *ptr; }

    template <typename V>
    operator StrongRef<V>()
        requires(std::is_convertible_v<T&, V&>)
    {
        return StrongRef<V>(ptr);
    }

    template <typename V>
    StrongRef<V> staticCast() const {
        return StrongRef<V>(std::static_pointer_cast<V>(ptr));
    }

    template <typename V>
    std::optional<StrongRef<V>> dynamicCast() const {
        return StrongRef<V>(std::dynamic_pointer_cast<V>(ptr));
    }

private:
    std::shared_ptr<T> ptr;
};

} // namespace gnev