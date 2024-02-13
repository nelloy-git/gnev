#pragma once

#include <concepts>

#include "gl/buffer/accessor/IBufferAccessor.hpp"
#include "util/IndexManager.hpp"
#include "util/InstanceLogger.hpp"

namespace gnev::gl {

template <typename T>
concept IsBufferViewMapData =
    (not std::is_pointer_v<T>);

template <IsBufferViewMapData T>
class BufferViewMap {
public:
    template <IsBufferViewMapData T>
    using Changer = std::function<void(T&)>;

    using FreeCallback = std::function<void(Ref<IBufferAccessor>&, const IndexGroup&)>;
    using MoveCallback = std::function<
        void(Ref<IBufferAccessor>&, unsigned src, unsigned dst, unsigned count)>;

    static Ref<BufferViewMap<T>> MakeDynamicStorage(unsigned capacity) {
        auto buffer = MakeSharable<gl::Buffer>();
        buffer->initStorage(capacity * sizeof(T), nullptr, GL_DYNAMIC_STORAGE_BIT);
        auto accessor = MakeSharable<gl::buffer::AccessorSubData>(buffer);
        return MakeSharable<BufferViewMap<T>>(accessor);
    }

    BufferViewMap(const Ref<IBufferAccessor>& accessor)
        : accessor{accessor}
        , index_manager(IndexManager::Make(accessor->getBuffer()->getSize() /
                                           sizeof(T))) {}

    static void DefaultFreeCallback(Ref<IBufferAccessor>& accessor,
                                    const IndexGroup& group) {}

    // callback can cleanup data or whatever
    auto reserve(unsigned count,
                 const FreeCallback& free_callback = &DefaultFreeCallback) {
        auto wrapper = [accessor, free_callback](const IndexGroup& group) {
            free_callback(accessor, group);
        };
        return index_manager->reserve(count, wrapper);
    }

    static void DefaultMoveCallback(Ref<IBufferAccessor>& accessor,
                                    const IndexGroup& dst,
                                    const IndexGroup& src) {
        auto buffer = accessor->getBuffer();
        buffer->copyTo(buffer,
                       src.first * sizeof(T),
                       dst.first * sizeof(T),
                       src.count * sizeof(T));
    }

    void optimize(const MoveCallback& move_callback = &DefaultMoveCallback) {
        auto wrapper = [accessor, move_callback](unsigned src, unsigned dst, unsigned count) {
            move_callback(accessor, src, dst, count);
        };
        index_manager->optimize(wrapper);
    }

    unsigned getCount() const { return index_manager->getUsedCapacity(); }

    void set(unsigned index, const T& value) {
        if (index_manager.isFree(index)) {
            InstanceLogger{}.Log<ERROR, "Index {} is not reserved">(index);
            return;
        }
        accessor->set(index * sizeof(T), sizeof(T), *value);
    }

    template <IsBufferViewMapData V>
    void set(unsigned index, const V& value, unsigned ptr_offset) {
        if (index_manager.isFree(index)) {
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
        accessor->set(index * sizeof(T) + ptr_offset, sizeof(V), *value);
    }

    void get(unsigned index, T& dst) const {
        if (index_manager.isFree(index)) {
            InstanceLogger{}.Log<ERROR, "Index {} is not reserved">(index);
            return;
        }
        accessor->get(index * sizeof(T), sizeof(T), *dst);
    }

    template <IsBufferViewMapData V>
    void get(unsigned index, V& dst, unsigned ptr_offset) const {
        if (index_manager.isFree(index)) {
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
        accessor->get(index * sizeof(T) + ptr_offset, sizeof(V), *dst);
    }

    void change(unsigned index, const Changer<T>& changer) const {
        if (index_manager.isFree(index)) {
            InstanceLogger{}.Log<ERROR, "Index {} is not reserved">(index);
            return;
        }
        auto wrapper = [&changer](gl::Buffer&, void* data, GLintptr) -> void {
            changer(*static_cast<T*>(data));
        };
        accessor->change(index * sizeof(T), sizeof(T), wrapper);
    }

    template <IsBufferViewMapData V>
    void change(unsigned index, const Changer<V>& changer, unsigned ptr_offset) const {
        if (index_manager.isFree(index)) {
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
    Ref<IndexManager> index_manager;
};

} // namespace gnev::gl