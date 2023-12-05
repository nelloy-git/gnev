#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>

namespace gnev {

template <typename T>
class Ref {
    template <typename V>
    friend class Ref;

    template <typename V>
    friend class WeakRef;

public:
    template <typename V>
    Ref<T>(const Ref<V>& other);

    template <typename V>
    Ref<T>(Ref<V>&& other);

    template <typename V>
    Ref(const std::shared_ptr<V>& ptr)
        requires(std::constructible_from<std::shared_ptr<T>, const std::shared_ptr<V>&>);

    template <typename V>
    Ref(std::shared_ptr<V>&& ptr)
        requires(std::constructible_from<std::shared_ptr<T>, std::shared_ptr<V> &&>);

    virtual ~Ref();

    T* operator->() const;
    T& operator*() const;
    operator T&() const;
    Ref<T>& operator=(const Ref<T>& other);
    bool operator==(const Ref<T>& other) const;

    std::shared_ptr<T> getPtr() const;

    template <typename V>
    Ref<V> staticCast() const;

    template <typename V>
    std::optional<Ref<V>> dynamicCast() const;

private:
    std::shared_ptr<T> ptr;
};

template <typename T, typename... Args>
Ref<T> MakeSharable(Args&&... args)
    requires(std::constructible_from<T, Args...>)
{
    auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
    return Ref<T>(ptr);
}

template <typename T>
template <typename V>
Ref<T>::Ref(const Ref<V>& other)
    : ptr(other.ptr) {}

template <typename T>
template <typename V>
Ref<T>::Ref(Ref<V>&& other)
    : ptr(std::move(other.ptr)) {}

template <typename T>
template <typename V>
Ref<T>::Ref(const std::shared_ptr<V>& ptr)
    requires(std::constructible_from<std::shared_ptr<T>, const std::shared_ptr<V>&>)
    : ptr(ptr) {
    if (not ptr) {
        throw std::logic_error("");
    }
}

template <typename T>
template <typename V>
Ref<T>::Ref(std::shared_ptr<V>&& ptr)
    requires(std::constructible_from<std::shared_ptr<T>, std::shared_ptr<V> &&>)
    : ptr(std::move(ptr)) {
    if (not ptr) {
        throw std::logic_error("");
    }
}

template <typename T>
Ref<T>::~Ref() {}

template <typename T>
T* Ref<T>::operator->() const {
    return ptr.operator->();
}

template <typename T>
T& Ref<T>::operator*() const {
    return ptr.operator*();
}

template <typename T>
Ref<T>::operator T&() const {
    return *ptr;
}

template <typename T>
Ref<T>& Ref<T>::operator=(const Ref<T>& other) {
    *ptr = *other.ptr;
    return *this;
}

template <typename T>
bool Ref<T>::operator==(const Ref<T>& other) const {
    return ptr == other.ptr;
}

template <typename T>
std::shared_ptr<T> Ref<T>::getPtr() const {
    return ptr;
}

template <typename T>
template <typename V>
Ref<V> Ref<T>::staticCast() const {
    return Ref<V>(std::static_pointer_cast<V>(ptr));
}

template <typename T>
template <typename V>
std::optional<Ref<V>> Ref<T>::dynamicCast() const {
    auto casted_ptr = std::dynamic_pointer_cast<V>(ptr);
    if (not casted_ptr) {
        return std::nullopt;
    }
    return Ref<V>(casted_ptr);
}

} // namespace gnev