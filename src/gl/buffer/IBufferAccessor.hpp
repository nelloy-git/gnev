// #pragma once

// #include <functional>

// #include "gl/Buffer.hpp"
// #include "gl/Ctx.hpp"
// #include "util/Export.hpp"

// namespace gnev::gl {

// class EXPORT IBufferAccessor {
// public:
//     using Changer = std::function<void(void*, GLintptr size)>;

//     virtual ~IBufferAccessor() = default;
    
//     virtual void bindBuffer(Buffer& buffer);

//     virtual void set(GLintptr offset, GLintptr size, const void* data) = 0;
//     virtual void get(GLintptr offset, GLintptr size, void* data) = 0;
//     virtual void change(GLintptr offset, GLintptr size, const Changer& changer) = 0;
//     virtual void copy(GLintptr src_offset, GLintptr dst_offset, GLintptr size) = 0;
// };

// } // namespace gnev::gl