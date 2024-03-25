// #include "MaterialPbrStorage.hpp"

// #include "gl/buffer/BufferAccessorSubData.hpp"

// namespace gnev {

// // MaterialPbrStorage::MaterialPbrStorage(std::unique_ptr<gl::IBufferAccessor>&& accessor,
// //                                        unsigned levels,
// //                                        unsigned width,
// //                                        unsigned height)
// //     : heap_buffer{std::move(accessor)}
// //     , tex_view({}) {}

// MaterialPbrStorage::MaterialPbrStorage(unsigned capacity,
//                                        unsigned levels,
//                                        unsigned width,
//                                        unsigned height)
//     : heap_buffer{initDynamicAccessor(capacity)}
//     , tex_view{
//           {gl::texture::IndexMapView::MakeArray2D(levels, width, height, capacity),
//            gl::texture::IndexMapView::MakeArray2D(levels, width, height, capacity),
//            gl::texture::IndexMapView::MakeArray2D(levels, width, height, capacity),
//            gl::texture::IndexMapView::MakeArray2D(levels, width, height, capacity)}} {}

// std::optional<MaterialPbr> MaterialPbrStorage::create() {
//     auto range_opt = heap_buffer.allocate(1);
//     if (not range_opt.has_value()) {
//         return std::nullopt;
//     }
//     return MaterialPbr{range_opt.value()};
// }

// std::unique_ptr<gl::IBufferAccessor>
// MaterialPbrStorage::initDynamicAccessor(unsigned capacity) {
//     auto buffer = std::make_unique<gl::Buffer>();
//     buffer->initStorage(capacity * sizeof(MaterialPbr::GL),
//                         nullptr,
//                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT |
//                             GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
//     return std::make_unique<gl::BufferAccessorSubData>(std::move(buffer));
// }

// } // namespace gnev