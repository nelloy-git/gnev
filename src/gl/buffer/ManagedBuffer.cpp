// #include "gl/buffer/ManagedBuffer.hpp"

// #include "gl/buffer/BufferAccessorMappedCoherent.hpp"
// #include "gl/buffer/BufferMemManagerFragmented.hpp"
// #include "magic_enum/magic_enum.hpp"

// namespace gnev::gl {

// ManagedBuffer::ManagedBuffer(std::size_t size,
//                              Accessor accessor_type,
//                              Manager manager_type) {
//     buffer = std::make_unique<Buffer>();

//     switch (accessor_type) {
//     case Accessor::MappedCoherent:
//         buffer->initStorage(size,
//                             nullptr,
//                             GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT |
//                                 GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
//         accessor = std::make_shared<gl::BufferRawAccessorMappedCoherent>(*buffer);
//         break;
//     default:
//         InstanceLogger{}
//             .Log<ERROR, "Unknown enum {} ({})">(magic_enum::enum_type_name<Accessor>(),
//                                                 int(accessor_type));
//     }

//     switch (manager_type) {
//     case Manager::Fragmented:
//         manager = std::make_unique<BufferMemManagerFragmented>(size);
//         break;
//     default:
//         InstanceLogger{}
//             .Log<ERROR, "Unknown enum {} ({})">(magic_enum::enum_type_name<Manager>(),
//                                                 int(manager_type));
//     }
// }

// } // namespace gnev::gl