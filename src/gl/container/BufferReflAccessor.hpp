#pragma once

#include <memory>

#include "gl/container/IBufferRawAccessor.hpp"
#include "util/Reflection.hpp"

namespace gnev::gl {

template <typename T>
static constexpr bool HasDefaultAlignmentV =
    sizeof(T) == sizeof(decltype(pfr::structure_to_tuple<T>(std::declval<T>())));

template <typename T>
concept IsReflectible =
    pfr::is_implicitly_reflectable_v<T, void> and HasDefaultAlignmentV<T>;

template <IsReflectible T>
class BufferReflAccessor {
public:
    using Meta = refl::Meta<T>;

    template <typename V>
    using Changer = std::function<void(V&)>;

    BufferReflAccessor(const std::shared_ptr<IBufferRawAccessor>& accessor,
                       unsigned base_offset)
        : accessor{accessor}
        , base_offset{base_offset} {}

    // bool set(const Struct& value) {
    //     return accessor->set(base_offset, sizeof(Struct), &value);
    // }

    // template <auto Key, auto... Next>
    // bool set(const refl::DeduceSubMeta<Meta, Key, Next...>& value) {
    //     // static constexpr std::size_t Offset =
    //     //     GetReflOffset<T, FirstFieldKey, FieldKeys...>();
    //     return accessor->set(base_offset + Meta::template Offset<Key, Next...>(),
    //     sizeof(value), &value);
    // }

    T get() const {
        T dst;
        accessor->get(base_offset, sizeof(T), &dst);
        return dst;
    }

    template <auto... Keys>
    Meta::template DeduceMember<Keys...>::type get() const {
        using R = typename Meta::template DeduceMember<Keys...>::type;
        R dst;
        accessor->get(base_offset, sizeof(R), &dst);
        return dst;
    }

    // template <auto FirstFieldKey, auto... FieldKeys>
    // bool get(ReflMeta<T, FirstFieldKey, FieldKeys...>::Type* dst) {
    //     static constexpr std::size_t Offset =
    //         GetReflOffset<T, FirstFieldKey, FieldKeys...>();
    //     return accessor->get(base_offset + Offset, sizeof(*dst), dst);
    // }

    // bool change(const Changer<T>& changer) const {
    //     return accessor->change(base_offset, sizeof(T), changer);
    // }

    // template <auto FirstFieldKey, auto... FieldKeys>
    // bool change(const Changer<typename ReflMeta<T, FirstFieldKey, FieldKeys...>::Type>&
    //                 changer) {
    //     static constexpr std::size_t Offset =
    //         GetReflOffset<T, FirstFieldKey, FieldKeys...>();
    //     static constexpr std::size_t Size =
    //         ReflMeta<T, FirstFieldKey, FieldKeys...>::Size;
    //     return accessor->change(base_offset + Offset, Size, changer);
    // }

    // bool copy(const BufferReflAccessor<T>& src) {
    //     return accessor->copy(src.base_offset, base_offset, sizeof(T));
    // }

    // template <auto FirstFieldKey, auto... FieldKeys>
    // bool copy(const BufferReflAccessor<T>& src) {
    //     static constexpr std::size_t Offset =
    //         GetReflOffset<T, FirstFieldKey, FieldKeys...>();
    //     static constexpr std::size_t Size =
    //         ReflMeta<T, FirstFieldKey, FieldKeys...>::Size;
    //     return accessor->copy(src.base_offset + Offset, base_offset + Offset, Size);
    // }

private:
    std::shared_ptr<IBufferRawAccessor> accessor;
    const unsigned base_offset;
};

} // namespace gnev::gl