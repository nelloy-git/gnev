#pragma once

#include <functional>

#include "util/Export.hpp"

#define GNEV_BUFFER_ACCESSOR_LOG(level, ...)                                             \
    BOOST_PP_CAT(GNEV_LOG_, level)                                                       \
    ("{}::{}" GNEV_ARGS_STR(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)),                        \
     GNEV_GET_TYPE_NAME(*this),                                                          \
     GNEV_GET_FUNC_NAME,                                                                 \
     ##__VA_ARGS__)

namespace gnev::gl {

class Buffer;

class EXPORT IBufferAccessor {
public:
    using Changer = std::function<void(void*, unsigned size)>;

    virtual ~IBufferAccessor() = default;

    // virtual Buffer& getBuffer() = 0;
    // virtual const Buffer& getBuffer() const = 0;

    virtual bool
    set(Buffer& buffer, unsigned offset, unsigned size, const void* data) = 0;
    virtual bool get(Buffer& buffer, unsigned offset, unsigned size, void* data) = 0;
    virtual bool
    change(Buffer& buffer, unsigned offset, unsigned size, const Changer& changer) = 0;
    virtual bool
    copy(Buffer& buffer, unsigned src_offset, unsigned dst_offset, unsigned size) = 0;
};

} // namespace gnev::gl