#pragma once

#include "gl/ReflStruct.hpp"
#include "gl/buffer/BufferMemBlock.hpp"
#include "gl/buffer/IBufferAccessor.hpp"
#include "util/InstanceLogger.hpp"

namespace gnev::gl {

template <IsReflStruct T>
class EXPORT ManagedSubBuffer {
public:
    friend class ManagedBuffer;

    unsigned getOffset() const {
        return mem.getOffset();
    }

    bool set(const T& value) { return setRaw(0, sizeof(T), &value); }

    template <auto... FieldKey>
    bool set(const auto& value) {
        return setSub<T, 0, FieldKey...>(value);
    }

    bool get(T* dst) const { return setRaw(0, sizeof(T), dst); }

    template <auto... FieldKey>
    bool get(auto* dst) {
        return getSub<T, 0, FieldKey...>(dst);
    }

private:
    ManagedSubBuffer(BufferMemBlock mem, std::weak_ptr<IBufferAccessor> weak_accessor)
        : mem{mem}
        , weak_accessor{weak_accessor} {}

    const BufferMemBlock mem;
    const std::weak_ptr<IBufferAccessor> weak_accessor;

    bool setRaw(GLintptr offset, GLintptr size, const void* value) {
        if (offset < 0) {
            InstanceLogger{}.Log<ERROR, "offset < 0 ({} < 0)">(offset);
            return false;
        }

        if (offset + size > mem.getSize()) {
            InstanceLogger{}
                .Log<ERROR,
                     "offset + size > mem.getSize() ({} + {} > {})">(offset,
                                                                     size,
                                                                     mem.getSize());
            return false;
        }

        auto accessor = weak_accessor.lock();
        if (not accessor) {
            InstanceLogger{}.Log<ERROR, "Accessor does not exist.">();
            return false;
        }

        accessor->set(mem.getOffset() + offset, size, value);
        return true;
    }

    template <IsReflStruct Parent, std::size_t Offset, auto Last>
    bool setSub(const Parent::Meta::template Field<Last>::Type& value) {
        using ChildInfo = Parent::Meta::template Field<Last>;
        using Child = ChildInfo::Type;
        return setRaw(Offset + ChildInfo::Offset, ChildInfo::Size, &value);
    }

    template <IsReflStruct Parent, std::size_t Offset, auto First, auto... FieldKey>
        requires(sizeof...(FieldKey) > 0 and
                 IsReflStruct<typename Parent::Meta::template Field<First>::Type>)
    bool setSub(const auto& value) {
        using ChildInfo = Parent::Meta::template Field<First>;
        using Child = ChildInfo::Type;
        return setSub<Child, Offset + ChildInfo::Offset, FieldKey...>(value);
    }

    bool getRaw(GLintptr offset, GLintptr size, void* dst) {
        if (offset < 0) {
            InstanceLogger{}.Log<ERROR, "offset < 0 ({} < 0)">(offset);
            return false;
        }

        if (offset + size > mem.getSize()) {
            InstanceLogger{}
                .Log<ERROR,
                     "offset + size > mem.getSize() ({} + {} > {})">(offset,
                                                                     size,
                                                                     mem.getSize());
            return false;
        }

        auto accessor = weak_accessor.lock();
        if (not accessor) {
            InstanceLogger{}.Log<ERROR, "Accessor does not exist.">();
            return false;
        }

        accessor->get(mem.getOffset() + offset, size, dst);
        return true;
    }

    template <IsReflStruct Parent, std::size_t Offset, auto Last>
    bool getSub(Parent::Meta::template Field<Last>::Type* dst) {
        using ChildInfo = Parent::Meta::template Field<Last>;
        using Child = ChildInfo::Type;
        return getRaw(Offset + ChildInfo::Offset, ChildInfo::Size, &dst);
    }

    template <IsReflStruct Parent, std::size_t Offset, auto First, auto... FieldKey>
        requires(sizeof...(FieldKey) > 0 and
                 IsReflStruct<typename Parent::Meta::template Field<First>::Type>)
    bool getSub(auto* dst) {
        using ChildInfo = Parent::Meta::template Field<First>;
        using Child = ChildInfo::Type;
        return getSub<Child, Offset + ChildInfo::Offset, FieldKey...>(dst);
    }
};

} // namespace gnev::gl