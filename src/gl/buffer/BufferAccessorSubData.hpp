// #pragma once

// #include "gl/buffer/IBufferAccessor.hpp"

// namespace gnev::gl {

// class EXPORT BufferAccessorSubData : public IBufferAccessor {
// public:
//     BufferAccessorSubData(Buffer& buffer);
//     ~BufferAccessorSubData() = default;

//     void set(GLintptr offset, GLintptr size, const void* data) override;
//     void get(GLintptr offset, GLintptr size, void* data) override;
//     void change(GLintptr offset, GLintptr size, const Changer& changer) override;
//     void copy(GLintptr src_offset, GLintptr dst_offset, GLintptr size) override;
// };

// } // namespace gnev::gl