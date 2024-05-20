#pragma once

#include <memory>

#include "gl/container/BufferPool.hpp"
#include "gl/container/BufferReflAccessor.hpp"

namespace gnev::gl {

template <IsTriviallyCopyable T,
          auto CapF = [](const BufferPool<T>& pool) { return 2 * pool.capacity(); }>
class BufferPoolElement {
public:
    using Pool = gnev::gl::BufferPool<T>;
    using Accessor = gnev::gl::BufferReflAccessor<T>;

    BufferPoolElement(const std::shared_ptr<Pool>& pool, const T& initial)
        : weak_pool(pool) {
        auto range = pool->pull(1);
        while (range.size != 1) {
            pool->reserve(CapF(*pool));
            range = pool->pull(1);
        }
        pool->fill(range, initial);
        pool_index = range.begin;
    }

    virtual ~BufferPoolElement() {
        auto pool = weak_pool.lock();
        if (pool) {
            pool->push(gnev::gl::Range{.begin = pool_index, .size = 1});
        }
    }

    unsigned index() const { return pool_index; }

    auto operator->() {
        auto pool = weak_pool.lock();
        if (not pool) {
            throw std::runtime_error("");
        }
        return std::make_unique<gnev::gl::BufferReflAccessor<T>>(pool->at(pool_index));
    }

    const auto operator->() const {
        auto pool = weak_pool.lock();
        if (not pool) {
            throw std::runtime_error("");
        }
        return std::make_unique<
            const gnev::gl::BufferReflAccessor<T>>(pool->at(pool_index));
    }

private:
    std::weak_ptr<Pool> weak_pool;
    unsigned pool_index;
};

} // namespace gnev::gl