#pragma once

#include <memory>

#include "gl/container/IBufferRawAccessor.hpp"
#include "util/refl/Struct.hpp"

namespace gnev::gl {

template <IsReflStruct T>
class BufferReflAccessor {
public:
    template <typename V>
    using Changer = std::function<void(V&)>;

    BufferReflAccessor(const std::shared_ptr<IBufferRawAccessor>& accessor,
                       unsigned base_offset)
        : accessor{accessor}
        , base_offset{base_offset} {}
    BufferReflAccessor(const BufferReflAccessor&) = delete;
    BufferReflAccessor(BufferReflAccessor&&) = delete;

    bool set(const T& value) { return accessor->set(base_offset, sizeof(T), &value); }

    template <auto FirstFieldKey, auto... FieldKeys>
    bool set(const typename ReflMeta<T, FirstFieldKey, FieldKeys...>::Type& value) {
        static constexpr std::size_t Offset =
            GetReflOffset<T, FirstFieldKey, FieldKeys...>();
        return accessor->set(base_offset + Offset, sizeof(value), &value);
    }

    bool get(T* dst) const { return accessor->get(base_offset, sizeof(T), dst); }

    template <auto FirstFieldKey, auto... FieldKeys>
    bool get(ReflMeta<T, FirstFieldKey, FieldKeys...>::Type* dst) {
        static constexpr std::size_t Offset =
            GetReflOffset<T, FirstFieldKey, FieldKeys...>();
        return accessor->get(base_offset + Offset, sizeof(*dst), dst);
    }

    bool change(const Changer<T>& changer) const {
        return accessor->change(base_offset, sizeof(T), changer);
    }

    template <auto FirstFieldKey, auto... FieldKeys>
    bool change(const Changer<typename ReflMeta<T, FirstFieldKey, FieldKeys...>::Type>&
                    changer) {
        static constexpr std::size_t Offset =
            GetReflOffset<T, FirstFieldKey, FieldKeys...>();
        static constexpr std::size_t Size =
            ReflMeta<T, FirstFieldKey, FieldKeys...>::Size;
        return accessor->change(base_offset + Offset, Size, changer);
    }

    bool copy(const BufferReflAccessor<T>& src) {
        return accessor->copy(src.base_offset, base_offset, sizeof(T));
    }

    template <auto FirstFieldKey, auto... FieldKeys>
    bool copy(const BufferReflAccessor<T>& src) {
        static constexpr std::size_t Offset =
            GetReflOffset<T, FirstFieldKey, FieldKeys...>();
        static constexpr std::size_t Size =
            ReflMeta<T, FirstFieldKey, FieldKeys...>::Size;
        return accessor->copy(src.base_offset + Offset, base_offset + Offset, Size);
    }

private:
    std::shared_ptr<IBufferRawAccessor> accessor;
    const unsigned base_offset;
};

} // namespace gnev::gl