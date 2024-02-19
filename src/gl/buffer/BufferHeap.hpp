#pragma once

#include <concepts>

#include "gl/buffer/IBufferAccessor.hpp"
#include "gl/buffer/IBufferElementManager.hpp"
#include "util/InstanceLogger.hpp"

namespace gnev::gl {

template <typename T>
concept IsBufferHeapData =
    (not std::is_pointer_v<T> and std::is_trivially_copyable_v<T>);

template <IsBufferHeapData T>
class BufferViewHeap {
public:
    static constexpr unsigned BlockSize = std::sizeof(T);

    template <IsBufferHeapData V>
    using Changer = std::function<void(V&)>;

    using FreeCallback = std::function<void(const IBufferAccessor&, const BufferElementBlock&)>;
    using MoveCallback = std::function<void(const IBufferAccessor&, unsigned src, unsigned dst, unsigned count)>;

    BufferViewHeap(const std::shared_ptr<IBufferAccessor>& accessor,
                  const std::shared_ptr<IBufferElementManager>& manager)
        : accessor{accessor}
        , manager{manager} {}

    auto reserve(unsigned count) {
        return manager->reserve(count);
    }

    void optimize() {
        auto wrapper = [this, move_callback](unsigned src, unsigned dst, unsigned count) {
            move_callback(accessor, src, dst, count);
        };
        index_manager->optimize(wrapper);
    }

    unsigned getCount() const { return index_manager->getUsedCapacity(); }

    void set(unsigned index, const T& value) {
        if (index_manager->isFree(index)) {
            InstanceLogger{}.Log<ERROR, "Index {} is not reserved">(index);
            return;
        }
        accessor->set(index * sizeof(T), sizeof(T), &value);
    }

    template <IsBufferHeapData V>
    void setSub(unsigned index, const V& value, unsigned ptr_offset) {
        if (index_manager->isFree(index)) {
            InstanceLogger{}.Log<ERROR, "Index {} is not reserved">(index);
            return;
        }
        if (ptr_offset + sizeof(V) >= sizeof(T)) {
            InstanceLogger{}
                .Log<ERROR,
                     "ptr_offset + sizeof(V) >= sizeof(T) ({} > {})">(ptr_offset +
                                                                          sizeof(V),
                                                                      sizeof(T));
            return;
        }
        accessor->set(index * sizeof(T) + ptr_offset, sizeof(V), &value);
    }

    void setSubGroup(IndexGroup group, unsigned first, unsigned count, const T* values) {
        if (index_manager->isFree(index)) {
            InstanceLogger{}.Log<ERROR, "Index {} is not reserved">(index);
            return;
        }
        if (ptr_offset + sizeof(V) >= sizeof(T)) {
            InstanceLogger{}
                .Log<ERROR,
                     "ptr_offset + sizeof(V) >= sizeof(T) ({} > {})">(ptr_offset +
                                                                          sizeof(V),
                                                                      sizeof(T));
            return;
        }
        accessor->set(index * sizeof(T) + ptr_offset, sizeof(V), &value);
    }

    void get(unsigned index, T& dst) const {
        if (index_manager->isFree(index)) {
            InstanceLogger{}.Log<ERROR, "Index {} is not reserved">(index);
            return;
        }
        accessor->get(index * sizeof(T), sizeof(T), &dst);
    }

    template <IsBufferHeapData V>
    void get(unsigned index, V& dst, unsigned ptr_offset) const {
        if (index_manager->isFree(index)) {
            InstanceLogger{}.Log<ERROR, "Index {} is not reserved">(index);
            return;
        }
        if (ptr_offset + sizeof(V) >= sizeof(T)) {
            InstanceLogger{}
                .Log<ERROR,
                     "ptr_offset + sizeof(V) >= sizeof(T) ({} > {})">(ptr_offset +
                                                                          sizeof(V),
                                                                      sizeof(T));
            return;
        }
        accessor->get(index * sizeof(T) + ptr_offset, sizeof(V), &dst);
    }

    void change(unsigned index, const Changer<T>& changer) const {
        if (index_manager->isFree(index)) {
            InstanceLogger{}.Log<ERROR, "Index {} is not reserved">(index);
            return;
        }
        auto wrapper = [&changer](gl::Buffer&, void* data, GLintptr) -> void {
            changer(*static_cast<T*>(data));
        };
        accessor->change(index * sizeof(T), sizeof(T), wrapper);
    }

    template <IsBufferHeapData V>
    void change(unsigned index, const Changer<V>& changer, unsigned ptr_offset) const {
        if (index_manager->isFree(index)) {
            InstanceLogger{}.Log<ERROR, "Index {} is not reserved">(index);
            return;
        }
        if (ptr_offset + sizeof(V) >= sizeof(T)) {
            InstanceLogger{}
                .Log<ERROR,
                     "ptr_offset + sizeof(V) >= sizeof(T) ({} > {})">(ptr_offset +
                                                                          sizeof(V),
                                                                      sizeof(T));
            return;
        }
        auto wrapper = [changer](gl::Buffer&, void* data, GLintptr) -> void {
            changer(*static_cast<V*>(data));
        };
        accessor->change(index * sizeof(T) + ptr_offset, sizeof(V), wrapper);
    }

private:
    Ref<IBufferAccessor> accessor;
    Ref<IBufferElementManager> manager;
};

} // namespace gnev::gl