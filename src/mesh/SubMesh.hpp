#pragma once

#include <algorithm>
#include <limits>
#include <vector>

#include "gl/container/BufferPool.hpp"
#include "gl/container/PoolController.hpp"

namespace gnev {

template <typename Index_, typename Vertex_>
class SubMesh {
public:
    using Index = Index_;
    using Vertex = Vertex_;

    SubMesh(gl::BufferPool<Index>& index_pool, gl::BufferPool<Vertex>& vertex_pool)
        : index_pool{index_pool}
        , vertex_pool{vertex_pool} {}

    virtual ~SubMesh() = default;

    void resizeIndexes(unsigned index_size) {
        auto new_index_range = pullRange(index_pool, index_size);
        moveRange(index_pool, index_range, new_index_range);
        index_range = new_index_range;
    }

    void resizeVertexes(unsigned vertex_size) {
        auto old_vertex_range = vertex_range;
        auto new_vertex_range = pullRange(vertex_pool, vertex_size);
        moveRange(vertex_pool, old_vertex_range, new_vertex_range);

        // Move index values
        if (old_vertex_range.begin != new_vertex_range.begin) {
            std::vector<Index> indexes{index_range.size};
            index_pool.get(index_range, indexes.data());
            std::for_each(indexes.begin(),
                          indexes.end(),
                          [&old_vertex_range, &new_vertex_range](Index& ind) {
                              ind = ind - old_vertex_range.begin + new_vertex_range.begin;
                          });
            index_pool.set(index_range, indexes.data());
        }
    }

    void set(const std::vector<Index>& indexes, const std::vector<Vertex>& vertexes) {
        resizeIndexes(indexes.size());
        std::vector<Index> indexes_with_offset = indexes;
        std::for_each(indexes_with_offset.begin(),
                      indexes_with_offset.end(),
                      [this](Index& val) { val += vertex_range.begin; });
        index_pool.set(index_range, indexes.data());
        resizeVertexes(vertexes.size());
        vertex_pool.set(vertex_range, vertexes.data());
    }

private:
    gl::BufferPool<Index>& index_pool;
    gl::BufferPool<Vertex>& vertex_pool;

    gl::Range index_range;
    gl::Range vertex_range;

    template <typename T>
    static inline gl::Range pullRange(gl::BufferPool<T>& pool, unsigned size) {
        auto range_opt = pool.pull(size);
        while (not range_opt.has_value()) {
            pool.reserve(2 * std::max(pool.capacity(), 1));
            range_opt = pool.pull(size);
        }
        return range_opt.value();
    }

    template <typename T>
    static inline void moveRange(gl::BufferPool<T>& pool, gl::Range src, gl::Range dst) {
        pool.copy(src.begin, dst.begin, std::min(src.size, dst.size));
        pool.push(dst, std::numeric_limits<Index>::max());
    }
};
} // namespace gnev