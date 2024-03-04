#pragma once

#include "gl/buffer/HeapBuffer.hpp"
#include "util/InstanceLogger.hpp"

namespace gnev::gl {

template <IsHeapBufferData T>
class HeapBufferRange {
public:
    HeapBufferRange(const std::shared_ptr<IBufferAccessor>& accessor, BufferRange range)
        : weak_accessor{accessor}
        , range{range} {}

    unsigned getOffset() const { return range.getOffset(); }

    void get(unsigned pos, T* dst, unsigned count = 1) const {
        if (pos + count > range.size()) {
            InstanceLogger{}
                .Log<ERROR, "pos + count > range.size() ({} > {})">(pos + count,
                                                                    range.size());
            return;
        }
        auto accessor = weak_accessor.lock();
        if (not accessor) {
            InstanceLogger{}.Log<ERROR, "Accessor does not exist.">();
            return;
        }
        accessor->get((range.getOffset() + pos) * sizeof(T), count * sizeof(T), dst);
    }

    template <typename V>
    void getSub(unsigned pos, V* dst, std::size_t offset) const {
        if (pos >= range.size()) {
            InstanceLogger{}.Log<ERROR, "pos >= range.size() ({} > {})">(pos,
                                                                         range.size());
            return;
        }
        if (sizeof(V) + offset > sizeof(T)) {
            InstanceLogger{}
                .Log<ERROR, "+ sizeof(V) + offs > sizeof(T) ({} > {})">(sizeof(V) +
                                                                            offset,
                                                                        sizeof(T));
            return;
        }
        auto accessor = weak_accessor.lock();
        if (not accessor) {
            InstanceLogger{}.Log<ERROR, "Storage does not exist.">();
            return;
        }
        accessor->get((range.getOffset() + pos) * sizeof(T) + offset, sizeof(V), dst);
    }

    void set(unsigned pos, const T* src, unsigned count = 1) {
        if (pos + count > range.size()) {
            InstanceLogger{}
                .Log<ERROR, "pos + count > range.size() ({} > {})">(pos + count,
                                                                    range.size());
            return;
        }
        auto accessor = weak_accessor.lock();
        if (not accessor) {
            InstanceLogger{}.Log<ERROR, "Storage does not exist.">();
            return;
        }
        accessor->set((range.getOffset() + pos) * sizeof(T), count * sizeof(T), src);
    }

    template <typename V>
    void setSub(unsigned pos, const V* src, std::size_t offset) {
        if (pos >= range.size()) {
            InstanceLogger{}.Log<ERROR, "pos >= range.size() ({} > {})">(pos,
                                                                         range.size());
            return;
        }
        if (sizeof(V) + offset > sizeof(T)) {
            InstanceLogger{}
                .Log<ERROR, "sizeof(V) + offset > sizeof(T) ({} > {})">(sizeof(V) +
                                                                            offset,
                                                                        sizeof(T));
            return;
        }
        auto accessor = weak_accessor.lock();
        if (not accessor) {
            InstanceLogger{}.Log<ERROR, "Storage does not exist.">();
            return;
        }
        accessor->set((range.getOffset() + pos) * sizeof(T) + offset, sizeof(V), src);
    }

    

private:
    std::weak_ptr<IBufferAccessor> weak_accessor;
    gl::BufferRange range;
};

} // namespace gnev::gl