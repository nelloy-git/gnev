// #pragma once

// #include "gl/VertexArray.hpp"
// #include "gl/buffer/HeapBuffer.hpp"
// #include "gl/buffer/IBufferAccessor.hpp"
// #include "mesh/base/Index.hpp"
// #include "mesh/base/Vertex.hpp"

// namespace gnev {

// template <base::IsVertex Vertex>
// class EXPORT Mesh {
// public:
//     template <base::IsVertex>
//     friend class SubMesh;

//     static constexpr GLuint BUFFER_BINDING = 0;

//     Mesh(std::unique_ptr<gl::IBufferAccessor>&& index_accessor,
//          std::unique_ptr<gl::IBufferAccessor>&& vertex_accessor)
//         : vao{std::make_unique<gl::VertexArray>()}
//         , indices{std::move(index_accessor)}
//         , vertices{std::move(vertex_accessor)}
//     {

//         vao->setElementBuffer(index_accessor->getBuffer());
//         vao->setVertexBuffer(BUFFER_BINDING,
//                              vertices.getAccessor().getBuffer(),
//                              0,
//                              sizeof(Vertex));

//         // TODO free callbacks
//     }

//     void bindAttribute(GLuint shader_loc, GLuint attrib_loc) {
//         static constexpr base::VertexInfo VERT_INFO = Vertex::info;

//         if (shader_loc >= GL_MAX_VERTEX_ATTRIBS || attrib_loc >= VERT_INFO.count) {
//             throw std::out_of_range("");
//         }

//         const base::VertexAttributeInfo& attr_info = VERT_INFO.attributes[attrib_loc];
//         vao->setAttributeBinding(shader_loc, BUFFER_BINDING);
//         vao->setAttributeFormat(shader_loc,
//                                 attr_info.elements,
//                                 attr_info.type,
//                                 attr_info.normalized,
//                                 VERT_INFO.offsets[attrib_loc]);
//         vao->enableAttribute(shader_loc);
//     }

//     void draw() const {
//         vao->bind();
//         gl::Ctx::Get().glDrawElements(GL_TRIANGLES,
//                                       indices.getAllocator().getMaxUsedCapacity(),
//                                       base::IndexEnum<unsigned>,
//                                       0);
//     }

// private:
//     std::unique_ptr<gl::VertexArray> vao;

//     gl::HeapBuffer<unsigned> indices;
//     gl::HeapBuffer<Vertex> vertices;
// };
// } // namespace gnev