#pragma once

#include <vector>

#include "gl/Buffer.hpp"
#include "gl/Ctx.hpp"
#include "gl/container/BufferReflAccessor.hpp"
#include "gl/container/IBufferRawAccessor.hpp"
#include "util/Logger.hpp"

namespace gnev::gl {

template <IsTriviallyCopyable T>
class BufferReflArray {
public:
    BufferReflArray(std::unique_ptr<IBufferRawAccessor>&& accessor_)
        : cap{initCap(accessor_->getBuffer())}
        , accessor{std::move(accessor_)} {}

    BufferReflArray(const BufferReflArray&) = delete;
    BufferReflArray(BufferReflArray&&) = default;

    ~BufferReflArray() {}

    inline std::size_t capacity() const { return cap; }

    enum class FillMethod {
        Massive,
        Iterate
    };

    inline void fill(const T& val, std::size_t batch = 0) {
        batch = batch == 0 ? capacity() : batch;

        std::size_t iters = capacity() / batch;
        std::vector<T> buffer{batch, val};
        for (std::size_t i = 0; i < iters; ++i) {
            accessor->set(0, batch * sizeof(T), buffer.data());
        }

        std::size_t left = capacity() - iters * batch;
        if (left == 0){
            return;
        }

        std::vector<T> left_buffer{left, val};
        accessor->set(0, left * sizeof(T), left_buffer.data());
    }

    inline BufferReflAccessor<T> at(std::size_t i) {
        return {accessor, static_cast<std::size_t>(i * sizeof(T))};
    }

    inline const BufferReflAccessor<T> at(std::size_t i) const {
        return {accessor, static_cast<std::size_t>(i * sizeof(T))};
    }

private:
    std::size_t cap;
    std::shared_ptr<IBufferRawAccessor> accessor;

    static std::size_t initCap(const Buffer& buffer) {
        auto buffer_size = buffer.getSize();
        if (buffer_size <= 0) {
            Ctx::Get()
                .getLogger()
                .logMsg<LogLevel::ERROR, "Invalid size of Buffer<{}>">(buffer.handle());
        }
        return std::max(buffer_size, 0) / sizeof(T);
    }
};

} // namespace gnev::gl