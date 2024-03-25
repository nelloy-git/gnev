#pragma once

#include "bufferStorage/IBufferAccessor.hpp"
#include "bufferStorage/IndexManager.hpp"
#include "gl/ReflStruct.hpp"
#include "util/InstanceLogger.hpp"

namespace gnev::gl {

template <IsReflStruct T>
class FixedSlot {
    template <IsReflStruct V>
    friend class FixedSlotStorage;

public:
    FixedSlot(const FixedSlot&) = delete;
    FixedSlot(FixedSlot&&) = delete;

    ~FixedSlot() {
        auto manager = weak_manager.lock();
        if (manager) {
            manager->freeIndex(index);
        }
    }

    unsigned getIndex() const { return index; }

    bool set(const T& value) { return setRaw(0, sizeof(T), &value); }

    template <auto... FieldKey>
    bool set(const auto& value) {
        return setSub<T, 0, FieldKey...>(value);
    }

    bool get(T* dst) { return getRaw(0, sizeof(T), dst); }

    template <auto... FieldKey>
    bool get(auto* value) {
        return getSub<T, 0, FieldKey...>(value);
    }

private:
    FixedSlot(unsigned index,
              const std::weak_ptr<IndexManager>& weak_manager,
              const std::shared_ptr<IBufferAccessor>& accessor)
        : index{index}
        , weak_manager{weak_manager}
        , accessor{accessor} {}

    unsigned index;
    std::weak_ptr<IndexManager> weak_manager;
    std::shared_ptr<IBufferAccessor> accessor;

    bool setRaw(unsigned offset, unsigned size, const void* value) {
        auto* buffer_ptr = accessor->getBoundBuffer();
        if (not buffer_ptr) {
            return false;
        }
        accessor->set(offset, size, value);
        return true;
    }

    template <IsReflStruct Parent,
              std::size_t Offset,
              auto FirstFieldKey,
              auto... FieldKeys>
        requires(IsReflStruct<typename Parent::Meta::template Field<FirstFieldKey>::Type>)
    inline bool setSub(const auto& value) {
        using ChildInfo = Parent::Meta::template Field<FirstFieldKey>;
        using Child = ChildInfo::Type;

        if constexpr (sizeof...(FieldKeys) == 0) {
            return setRaw(Offset + ChildInfo::Offset, ChildInfo::Size, &value);
        } else {
            return setSub<Child, Offset + ChildInfo::Offset, FieldKeys...>(value);
        }
    }

    bool getRaw(unsigned offset, unsigned size, void* value) {
        auto* buffer_ptr = accessor->getBoundBuffer();
        if (not buffer_ptr) {
            return false;
        }
        accessor->get(offset, size, value);
        return true;
    }

    template <IsReflStruct Parent,
              std::size_t Offset,
              auto FirstFieldKey,
              auto... FieldKeys>
        requires(IsReflStruct<typename Parent::Meta::template Field<FirstFieldKey>::Type>)
    inline bool getSub(auto* value) {
        using ChildInfo = Parent::Meta::template Field<FirstFieldKey>;
        using Child = ChildInfo::Type;

        if constexpr (sizeof...(FieldKeys) == 0) {
            return getRaw(Offset + ChildInfo::Offset, ChildInfo::Size, value);
        } else {
            return getSub<Child, Offset + ChildInfo::Offset, FieldKeys...>(value);
        }
    }
};

} // namespace gnev::gl