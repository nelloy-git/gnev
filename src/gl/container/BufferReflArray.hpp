// #pragma once

// #include <vector>

// #include "gl/Buffer.hpp"
// #include "gl/Handler.hpp"
// #include "gl/container/BufferReflAccessor.hpp"
// #include "gl/container/IBufferRawAccessor.hpp"

// namespace gnev::gl {

// template <IsTriviallyCopyable T>
// class BufferReflArray {
// public:
//     BufferReflArray(std::unique_ptr<IBufferAccessor>&& accessor_)
//         : cap{initCap(accessor_->getBuffer())}
//         , accessor{std::move(accessor_)} {}

//     BufferReflArray(const BufferReflArray&) = delete;
//     BufferReflArray(BufferReflArray&&) = default;

//     ~BufferReflArray() {}

//     inline std::size_t capacity() const { return cap; }

//     virtual std::size_t size() const { return cap; }

//     enum class FillMethod {
//         Massive,
//         Iterate
//     };

//     inline void fill(const T& val, std::size_t batch = 0) {
//         batch = batch == 0 ? capacity() : batch;

//         std::size_t iters = capacity() / batch;
//         std::vector<T> buffer{batch, val};
//         for (std::size_t i = 0; i < iters; ++i) {
//             accessor->set(0, batch * sizeof(T), buffer.data());
//         }

//         std::size_t left = capacity() - iters * batch;
//         if (left == 0) {
//             return;
//         }

//         std::vector<T> left_buffer{left, val};
//         accessor->set(0, left * sizeof(T), left_buffer.data());
//     }

//     inline BufferReflAccessor<T> at(std::size_t i) {
//         return {accessor, static_cast<std::size_t>(i * sizeof(T))};
//     }

//     inline const BufferReflAccessor<T> at(std::size_t i) const {
//         return {accessor, static_cast<std::size_t>(i * sizeof(T))};
//     }

// private:
//     std::size_t cap;
//     std::shared_ptr<IBufferAccessor> accessor;

//     static std::size_t initCap(const Buffer* buffer) {
//         if (not buffer) {
//             GNEV_LOG(WARNING, "Empty Buffer pointer");
//             return 0;
//         }

//         auto elements = buffer->getSize() / sizeof(T);
//         if (elements == 0) {
//             GNEV_LOG(WARNING,
//                      "Size of Buffer#{} is not enough for at least one element",
//                      buffer->handle());
//         }

//         return elements;
//     }
// };

// } // namespace gnev::gl