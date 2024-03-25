// #include "gl/buffer/BufferAccessorMappedCoherent.hpp"

// #include "gl/fmt/BitFlags.hpp"
// #include "util/InstanceLogger.hpp"

// namespace gnev::gl {

// BufferRawAccessorMappedCoherent::BufferRawAccessorMappedCoherent(Buffer& buffer)
//     : IBufferAccessor{buffer} {

//     if (not buffer.isStorage()) {
//         InstanceLogger{}.Log<ERROR, "Buffer<{}> is not storage">(buffer.handle());
//     }

//     GLbitfield storage_flags = buffer.getStorageFlags();
//     if (not(storage_flags & GL_MAP_COHERENT_BIT) and
//         not(storage_flags & GL_CLIENT_STORAGE_BIT)) {
//         InstanceLogger{}
//             .Log<ERROR,
//                  "Buffer<{}> has invalid storage flags {}">(buffer.handle(),
//                                                             fmt::BitFlags{
//                                                                 storage_flags,
//                                                                 fmt::BitFlags::Group::
//                                                                     glBufferStorage});
//     }

//     map = static_cast<GLbyte*>(buffer.mapRange(0,
//                                                buffer.getSize(),
//                                                GL_MAP_READ_BIT | GL_MAP_WRITE_BIT |
//                                                    GL_MAP_PERSISTENT_BIT |
//                                                    GL_MAP_COHERENT_BIT));
// }

// BufferRawAccessorMappedCoherent::~BufferRawAccessorMappedCoherent() {
//     getBuffer().unmap();
// }

// void BufferRawAccessorMappedCoherent::set(GLintptr offset,
//                                           GLintptr size,
//                                           const void* src) {
//     std::memcpy(map + offset, src, size);
// }

// void BufferRawAccessorMappedCoherent::get(GLintptr offset, GLintptr size, void* dst) {
//     std::memcpy(dst, map + offset, size);
// }

// void BufferRawAccessorMappedCoherent::change(GLintptr offset,
//                                              GLintptr size,
//                                              const Changer& changer) {
//     changer(map + offset, size);
// }

// void BufferRawAccessorMappedCoherent::copy(GLintptr src_offset,
//                                            GLintptr dst_offset,
//                                            GLintptr size) {
//     std::memcpy(map + dst_offset, map + src_offset, size);
// }

// } // namespace gnev::gl