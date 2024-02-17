#pragma once

#include <functional>

#include "gl/Buffer.hpp"
#include "gl/Ctx.hpp"
#include "util/Export.hpp"
#include "util/Ref.hpp"

namespace gnev::gl {

class EXPORT IBufferAccessor {
public:
    using Changer = std::function<void(void*, GLintptr size)>;

    virtual ~IBufferAccessor() = default;

    virtual const Ref<gl::Buffer>& getBuffer() const = 0;

    virtual void set(GLintptr offset, GLintptr size, const void* data) = 0;
    virtual void get(GLintptr offset, GLintptr size, void* data) = 0;
    virtual void change(GLintptr offset, GLintptr size, const Changer& changer) = 0;
};

} // namespace gnev::gl