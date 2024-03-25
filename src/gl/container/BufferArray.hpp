#pragma once

#include "gl/Buffer.hpp"
#include "gl/ReflStruct.hpp"
#include "gl/container/BufferReflStructAccessor.hpp"
#include "gl/container/IBufferRawAccessor.hpp"

namespace gnev::gl {

template <IsReflStruct T>
class BufferArray {
public:
    struct Elem {
        const unsigned index;
        BufferReflStructAccessor<T> accessor;
    };

    BufferArray(std::unique_ptr<Buffer>&& buffer_,
                std::unique_ptr<IBufferRawAccessor>&& accessor_)
        : size{static_cast<unsigned>(buffer_->getSize() / sizeof(T))}
        , buffer{std::move(buffer_)}
        , accessor{std::move(accessor_)} {
        accessor->bindBuffer(buffer.get());
    }

    BufferArray(const BufferArray&) = delete;
    BufferArray(BufferArray&&) = delete;

    ~BufferArray() { accessor->bindBuffer(nullptr); }

    Elem operator[](unsigned i) {
        return Elem{.index = i,
                    .accessor = {accessor, static_cast<unsigned>(i * sizeof(T))}};
    }

private:
    const unsigned size;
    std::unique_ptr<gl::Buffer> buffer;
    std::shared_ptr<IBufferRawAccessor> accessor;
};

} // namespace gnev::gl