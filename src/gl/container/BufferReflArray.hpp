#pragma once

#include <stdexcept>

#include "gl/Buffer.hpp"
#include "gl/container/BufferReflAccessor.hpp"
#include "gl/container/IBufferRawAccessor.hpp"

namespace gnev::gl {

template <IsReflStruct T>
class BufferReflArray {
public:
    BufferReflArray(std::unique_ptr<IBufferRawAccessor>&& accessor_)
        : cap{static_cast<unsigned>(accessor_->getBuffer().getSize() / sizeof(T))}
        , accessor{std::move(accessor_)} {
    }

    BufferReflArray(const BufferReflArray&) = delete;
    BufferReflArray(BufferReflArray&&) = default;

    ~BufferReflArray() {}

    unsigned size() const { return cap; }

    BufferReflAccessor<T> operator[](unsigned i) {
        return {accessor, static_cast<unsigned>(i * sizeof(T))};
    }

    const BufferReflAccessor<T> operator[](unsigned i) const {
        return {accessor, static_cast<unsigned>(i * sizeof(T))};
    }

    BufferReflAccessor<T> at(unsigned i) {
        if (i >= cap) {
            throw std::out_of_range("");
        }
        return this->operator[](i);
    }

    const BufferReflAccessor<T> at(unsigned i) const {
        if (i >= cap) {
            throw std::out_of_range("");
        }
        return this->operator[](i);
    }

private:
    unsigned cap;
    std::shared_ptr<IBufferRawAccessor> accessor;
};

} // namespace gnev::gl