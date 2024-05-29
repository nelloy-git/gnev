#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include "gl/Buffer.hpp"
#include "gl/Handler.hpp"
#include "gl/SyncFence.hpp"
#include "gl/container/BufferReflAccessor.hpp"
#include "gl/container/IBufferAccessor.hpp"
#include "gl/container/IBufferAllocator.hpp"
#include "gl/container/IBufferContainer.hpp"
#include "gl/container/PoolController.hpp"
#include "gl/enum/SyncStatus.hpp"
#include "quill/bundled/fmt/format.h"

#define GNEV_BUFFER_POOL_LOG(level, ...)                                                 \
    BOOST_PP_CAT(GNEV_LOG_, level)                                                       \
    ("{}#{}::{}" GNEV_ARGS_STR(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)),                     \
     GNEV_GET_TYPE_NAME(*this),                                                          \
     fmtquill::to_string(this->getBuffer()),                                             \
     GNEV_GET_FUNC_NAME,                                                                 \
     ##__VA_ARGS__)

namespace gnev::gl {

template <IsTriviallyCopyable T>
class EXPORT BufferPool : public IBufferContainer<T> {
public:
    BufferPool(std::unique_ptr<IBufferAccessor>&& accessor,
               std::unique_ptr<IBufferAllocator>&& allocator,
               unsigned size)
        : controller(size)
        , buffer_{allocator->allocate(size * sizeof(T), nullptr)}
        , accessor_{std::move(accessor)}
        , allocator_{std::move(allocator)} {}

    gl::Buffer& getBuffer() override { return *buffer_; }

    const gl::Buffer& getBuffer() const override { return *buffer_; }

    inline unsigned capacity() const { return controller.capacity(); }

    inline unsigned size() const override { return controller.size(); }

    Range pull(unsigned size) {
        GNEV_BUFFER_POOL_LOG(L1, size);
        auto range = controller.pull(size);
        GNEV_LOG(L1, "\treturn {}", range);
        return range;
    }

    void push(Range range) {
        GNEV_BUFFER_POOL_LOG(L1, range);
        controller.push(range);
    };

    void reserve(unsigned cap) {
        static constexpr unsigned wait_sync_ns = 1000000000;

        GNEV_BUFFER_POOL_LOG(L1, cap);

        auto prev_size = controller.size();
        auto new_cap = std::max(prev_size, cap);
        if (new_cap == prev_size) {
            return;
        }

        controller.reserve(new_cap);
        auto resized_buffer = allocator_->allocate(new_cap * sizeof(T), nullptr);
        buffer_->copyTo(*resized_buffer, 0, 0, prev_size * sizeof(T));
        auto sync_status = SyncFence{}.wait();
        if (sync_status == SyncStatus::TIMEOUT_EXPIRED or
            sync_status == SyncStatus::WAIT_FAILED) {
            GNEV_LOG_WARNING("\tFailed sync buffer resize with reason: {}", sync_status);
        }
        buffer_.swap(resized_buffer);
    }

    void fill(Range range, const T& val, unsigned batch = 0) {
        GNEV_BUFFER_POOL_LOG(L1, range, GNEV_GET_TYPE_NAME(val), batch);
        batch = batch == 0 ? range.size : batch;

        unsigned iters = range.size / batch;
        unsigned batch_size = batch * sizeof(T);
        std::vector<T> vals{batch, val};
        for (unsigned i = 0; i < iters; ++i) {
            unsigned offset = (range.begin + i * batch) * sizeof(T);
            accessor_->set(*buffer_, offset, batch_size, vals.data());
        }

        unsigned left = range.size - iters * batch;
        if (left == 0) {
            return;
        }

        unsigned offset = (range.begin + iters * batch) * sizeof(T);
        accessor_->set(*buffer_, offset, batch_size, vals.data());
    }

    bool set(Range range, const T* src) {
        GNEV_BUFFER_POOL_LOG(L1, range, static_cast<const void*>(src));
        if (not controller.isInUse(range)) {
            return false;
        }
        return accessor_->set(*buffer_,
                              range.begin * sizeof(T),
                              range.size * sizeof(T),
                              src);
    }

    bool get(Range range, T* dst, unsigned batch = 0) const {
        GNEV_BUFFER_POOL_LOG(L1, range, static_cast<const void*>(dst), batch);
        if (not controller.isInUse(range)) {
            return false;
        }
        return accessor_->get(*buffer_,
                              range.begin * sizeof(T),
                              range.size * sizeof(T),
                              dst);
    }

    bool copy(unsigned src, unsigned dst, unsigned size) {
        GNEV_BUFFER_POOL_LOG(L1, src, dst, size);
        if (not controller.isInUse(Range{.begin = src, .size = size}) or
            not controller.isInUse(Range{.begin = dst, .size = size})) {
            return false;
        }
        return accessor_->copy(*buffer_,
                               src * sizeof(T),
                               dst * sizeof(T),
                               size * sizeof(T));
    }

    BufferReflAccessor<T> at(unsigned i) {
        if (not controller.isInUse(i)) {
            throw std::out_of_range("");
        }
        return {*buffer_, *accessor_, static_cast<unsigned>(i * sizeof(T))};
    }

    const BufferReflAccessor<T> at(unsigned i) const {
        if (not controller.isInUse(i)) {
            throw std::out_of_range("");
        }
        return {*buffer_, *accessor_, static_cast<unsigned>(i * sizeof(T))};
    }

private:
    PoolController controller;

    std::unique_ptr<Buffer> buffer_;
    std::unique_ptr<IBufferAccessor> accessor_;
    std::unique_ptr<IBufferAllocator> allocator_;
};

} // namespace gnev::gl