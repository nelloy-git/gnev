#pragma once

#include <concepts>

#include "gl/buffer/accessor/IBufferAccessor.hpp"
#include "util/InstanceLogger.hpp"

namespace gnev::gl {

template <typename T>
concept IsBufferViewArrayData =
    not std::is_pointer_v<T> and std::is_trivially_constructible_v<T>;

template <IsBufferViewArrayData T>
class BufferViewArray {
public:
    template <IsBufferViewArrayData V>
    using Changer = std::function<void(V&)>;

    BufferViewArray(const Ref<IBufferAccessor>& accessor)
        : accessor{accessor}
        , capacity{accessor->getBuffer()->getSize() / sizeof(T)} {}

    void set(unsigned index, const T& value) {
        if (index >= capacity) {
            InstanceLogger{}.Log<ERROR, "out of range index ({} >= {})">(index, capacity);
        }
        accessor->set(index * sizeof(T), sizeof(T), &value);
    }

    template <IsBufferViewArrayData V>
    void set(unsigned index, const V& value, unsigned ptr_offset) {
        if (index >= capacity) {
            InstanceLogger{}.Log<ERROR, "out of range index ({} >= {})">(index, capacity);
        }
        if (ptr_offset + sizeof(V) >= sizeof(T)) {
            InstanceLogger{}
                .Log<ERROR,
                     "ptr_offset + sizeof(V) >= sizeof(T) ({} > {})">(ptr_offset +
                                                                          sizeof(V),
                                                                      sizeof(T));
        }
        accessor->set(index * sizeof(T) + ptr_offset, sizeof(V), &value);
    }

    void get(unsigned index, T& dst) const {
        if (index >= capacity) {
            InstanceLogger{}.Log<ERROR, "out of range index ({} >= {})">(index, capacity);
        }
        accessor->get(index * sizeof(T), sizeof(T), &dst);
    }

    template <IsBufferViewArrayData V>
    void get(unsigned index, V& dst, unsigned ptr_offset) const {
        if (index >= capacity) {
            InstanceLogger{}.Log<ERROR, "out of range index ({} >= {})">(index, capacity);
        }
        if (ptr_offset + sizeof(V) >= sizeof(T)) {
            InstanceLogger{}
                .Log<ERROR,
                     "ptr_offset + sizeof(V) >= sizeof(T) ({} > {})">(ptr_offset +
                                                                          sizeof(V),
                                                                      sizeof(T));
        }
        accessor->get(index * sizeof(T) + ptr_offset, sizeof(V), &dst);
    }

    void change(unsigned index, const Changer<T>& changer) {
        if (index >= capacity) {
            InstanceLogger{}.Log<ERROR, "out of range index ({} >= {})">(index, capacity);
        }
        auto wrapper = [changer](gl::Buffer&, void* data, GLintptr) -> void {
            changer(*static_cast<T*>(data));
        };
        accessor->change(index * sizeof(T), sizeof(T), wrapper);
    }

    template <IsBufferViewArrayData V>
    void change(unsigned index, const Changer<V>& changer, unsigned ptr_offset) {
        if (index >= capacity) {
            InstanceLogger{}.Log<ERROR, "out of range index ({} >= {})">(index, capacity);
        }
        if (ptr_offset + sizeof(V) >= sizeof(T)) {
            InstanceLogger{}
                .Log<ERROR,
                     "ptr_offset + sizeof(V) >= sizeof(T) ({} > {})">(ptr_offset +
                                                                          sizeof(V),
                                                                      sizeof(T));
        }
        auto wrapper = [changer](gl::Buffer&, void* data, GLintptr) -> void {
            changer(*static_cast<V*>(data));
        };
        accessor->change(index * sizeof(T) + ptr_offset, sizeof(V), wrapper);
    }

private:
    Ref<IBufferAccessor> accessor;
    unsigned capacity;
};

} // namespace gnev::gl