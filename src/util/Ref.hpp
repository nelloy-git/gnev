#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>

namespace gnev {

template <typename T>
using Ptr = std::shared_ptr<T>;

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
    Ref(const Ptr<V>& ptr)
        requires(std::constructible_from<Ptr<T>, const Ptr<V>&>);

    template <typename V>
    Ref(Ptr<V>&& ptr)
        requires(std::constructible_from<Ptr<T>, Ptr<V> &&>);

    virtual ~Ref();

    T* operator->() const;
    T& operator*() const;
    operator T&() const;
    template <typename V>
    operator Ptr<V>() const;
    bool operator==(const Ref<T>& other) const;

    Ptr<T> getPtr() const;

    template <typename V>
    Ptr<V> getPtr() const;

    template <typename V>
    Ref<V> staticCast() const;

    template <typename V>
    std::optional<Ref<V>> dynamicCast() const;

    void swap(Ref<T>& other);

private:
    Ptr<T> ptr;
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
Ref<T>::Ref(const Ptr<V>& ptr)
    requires(std::constructible_from<Ptr<T>, const Ptr<V>&>)
    : ptr(ptr) {
    if (not this->ptr) {
        throw std::logic_error("");
    }
}

template <typename T>
template <typename V>
Ref<T>::Ref(Ptr<V>&& ptr)
    requires(std::constructible_from<Ptr<T>, Ptr<V> &&>)
    : ptr(std::move(ptr)) {
    if (not this->ptr) {
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
template <typename V>
Ref<T>::operator Ptr<V>() const {
    return ptr;
}

template <typename T>
bool Ref<T>::operator==(const Ref<T>& other) const {
    return ptr == other.ptr;
}

template <typename T>
Ptr<T> Ref<T>::getPtr() const {
    return ptr;
}

template <typename T>
template <typename V>
Ptr<V> Ref<T>::getPtr() const {
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

template<typename T>
void Ref<T>::swap(Ref<T>& other){
    this->ptr.swap(other.ptr);
}

} // namespace gnev