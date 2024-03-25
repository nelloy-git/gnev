#pragma once

#include <memory>

#include "gl/ReflStruct.hpp"
#include "gl/container/IBufferRawAccessor.hpp"

namespace gnev::gl {

template <IsReflStruct T>
class BufferReflStructAccessor {
public:
    template <typename V>
    using Changer = std::function<void(V&)>;

    BufferReflStructAccessor(const std::shared_ptr<IBufferRawAccessor>& accessor,
                             unsigned base_offset)
        : accessor{accessor}
        , base_offset{base_offset} {}

    bool set(const T& value) { return accessor->set(base_offset, sizeof(T), &value); }

    template <auto FirstFieldKey, auto... FieldKeys>
    bool set(const ReflStructDeduceMeta<T, FirstFieldKey, FieldKeys...>::Type& value) {
        static constexpr std::size_t Offset =
            ReflStructDeduceOffset<T, FirstFieldKey, FieldKeys...>();
        return accessor->set(base_offset + Offset, sizeof(value), &value);
    }

    bool get(T* dst) const { return accessor->get(base_offset, sizeof(T), dst); }

    template <auto FirstFieldKey, auto... FieldKeys>
    bool get(ReflStructDeduceMeta<T, FirstFieldKey, FieldKeys...>::Type* dst) {
        static constexpr std::size_t Offset =
            ReflStructDeduceOffset<T, FirstFieldKey, FieldKeys...>();
        return accessor->set(base_offset + Offset, sizeof(*dst), dst);
    }

    bool change(const Changer<T>& changer) const {
        return accessor->change(base_offset, sizeof(T), changer);
    }

    template <auto FirstFieldKey, auto... FieldKeys>
    bool
    change(const Changer<
           typename ReflStructDeduceMeta<T, FirstFieldKey, FieldKeys...>::Type> changer) {
        static constexpr std::size_t Offset =
            ReflStructDeduceOffset<T, FirstFieldKey, FieldKeys...>();
        return accessor->set(base_offset + Offset,
                             ReflStructDeduceMeta<T, FirstFieldKey, FieldKeys...>::Size,
                             changer);
    }

    bool copy(const BufferReflStructAccessor<T>& src) {
        return accessor->copy(src.base_offset, base_offset, sizeof(T));
    }

    template <auto FirstFieldKey, auto... FieldKeys>
    bool copy(const BufferReflStructAccessor<T>& src) {
        static constexpr std::size_t Offset =
            ReflStructDeduceOffset<T, FirstFieldKey, FieldKeys...>();
        return accessor->copy(src.base_offset + Offset,
                              base_offset + Offset,
                              ReflStructDeduceMeta<T, FirstFieldKey, FieldKeys...>::Size);
    }

private:
    std::shared_ptr<IBufferRawAccessor> accessor;
    const unsigned base_offset;
};

} // namespace gnev::gl