#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>

namespace gnev {

template <typename T>
class Ref {
    template <typename V>
    friend class WeakRef;

public:
    template <typename... Args>
    static Ref<T> Make(Args&&... args)
        requires(std::constructible_from<T, Args...>);

    template <typename V>
    Ref(const std::shared_ptr<V>& ptr)
        requires(std::constructible_from<std::shared_ptr<T>, std::shared_ptr<V>>);

    template <typename... Args>
    Ref<T>(Args&&... args)
        requires(std::constructible_from<T, Args...>);

    virtual ~Ref();

    T* operator->() const;
    operator T&() const;

    template <typename V>
    operator Ref<V>() const;

    template <typename V>
    Ref<V> staticCast() const;

    template <typename V>
    std::optional<Ref<V>> dynamicCast() const;

private:
    std::shared_ptr<T> ptr;
};

template <typename T>
template <typename... Args>
Ref<T> Ref<T>::Make(Args&&... args)
    requires(std::constructible_from<T, Args...>)
{
    auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
    return Ref<T>(ptr);
}

template <typename T>
template <typename V>
Ref<T>::Ref(const std::shared_ptr<V>& ptr)
    requires(std::constructible_from<std::shared_ptr<T>, std::shared_ptr<V>>)
    : ptr(ptr) {
    if (not ptr) {
        throw std::logic_error("");
    }
}

template <typename T>
template <typename... Args>
Ref<T>::Ref(Args&&... args)
    requires(std::constructible_from<T, Args...>)
{
    auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
    return Ref<T>(ptr);
}

template <typename T>
Ref<T>::~Ref() {}

template <typename T>
T* Ref<T>::operator->() const {
    return ptr.get();
}

template <typename T>
Ref<T>::operator T&() const {
    return *ptr;
}

template <typename T>
template <typename V>
Ref<T>::operator Ref<V>() const {
    return Ref<V>(ptr);
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