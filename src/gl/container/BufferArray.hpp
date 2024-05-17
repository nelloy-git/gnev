#pragma once

#include <initializer_list>
#include <memory>

#include "gl/container/BufferReflAccessor.hpp"
#include "gl/container/IBufferAccessor.hpp"
#include "gl/container/IBufferAllocator.hpp"
#include "gl/container/IBufferContainer.hpp"

namespace gnev::gl {

template <IsTriviallyCopyable T>
class BufferArray : IBufferContainer {
public:
    BufferArray(std::unique_ptr<IBufferAccessor>&& accessor,
                std::unique_ptr<IBufferAllocator>&& allocator,
                unsigned size,
                const T& initial = T{},
                unsigned batch = 0)
        : size_{size}
        , buffer_{allocator->allocate(size * sizeof(T), nullptr)}
        , accessor_{std::move(accessor)}
        , allocator_{std::move(allocator)} {
        fill(initial, batch);
    }

    BufferArray(std::unique_ptr<IBufferAccessor>&& accessor,
                std::unique_ptr<IBufferAllocator>&& allocator,
                std::initializer_list<T> initial,
                unsigned size = 0)
        : size_{std::max<unsigned>(initial.size(), size)}
        , buffer_{allocator->allocate(std::max<unsigned>(initial.size(), size) * sizeof(T), initial.begin())}
        , accessor_{std::move(accessor)}
        , allocator_{std::move(allocator)} {
    }

    GLuint handle() const override { return buffer_->handle(); }

    inline unsigned capacity() const { return size_; }

    inline unsigned size() const { return size_; }

    inline void fill(const T& val, std::size_t batch = 0) {
        batch = batch == 0 ? capacity() : batch;

        std::size_t iters = capacity() / batch;
        std::vector<T> buffer{batch, val};
        for (std::size_t i = 0; i < iters; ++i) {
            accessor_->set(0, batch * sizeof(T), buffer.data());
        }

        std::size_t left = capacity() - iters * batch;
        if (left == 0) {
            return;
        }

        std::vector<T> left_buffer{left, val};
        accessor_->set(0, left * sizeof(T), left_buffer.data());
    }

    inline BufferReflAccessor<T> at(std::size_t i) {
        return {*buffer_, *accessor_, static_cast<std::size_t>(i * sizeof(T))};
    }

    inline const BufferReflAccessor<T> at(std::size_t i) const {
        return {*buffer_, *accessor_, static_cast<std::size_t>(i * sizeof(T))};
    }

private:
    unsigned size_;

    std::unique_ptr<Buffer> buffer_;
    std::unique_ptr<IBufferAccessor> accessor_;
    std::unique_ptr<IBufferAllocator> allocator_;
};

} // namespace gnev::gl