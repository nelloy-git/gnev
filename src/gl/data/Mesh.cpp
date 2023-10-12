#include "gl/data/Mesh.hpp"

#include <cstddef>
#include <stdexcept>

#include "gl/data/Index.hpp"
#include "gl/data/VertexAttributeInfo.hpp"

using namespace gnev::gl::data;

MeshedBuffer::MeshedBuffer(const VertexInfo& vertex_info)
    : Mesh()
    , vertex_info(vertex_info)
    , index_enum(0) {}

MeshedBuffer::~MeshedBuffer() {}

void MeshedBuffer::bindElements(const gl::Buffer& index_buffer, GLenum index_type) {
    glVertexArrayElementBuffer(index_buffer.handle());
    index_enum = index_type;
}

void MeshedBuffer::bindVertices(const gl::Buffer& buffer) {
    glVertexArrayVertexBuffer(0, buffer.handle(), 0, vertex_info.size);
}

void MeshedBuffer::bindAttribute(Loc shader_loc, Loc mesh_loc) {
    if (shader_loc >= GL_MAX_VERTEX_ATTRIBS || mesh_loc >= vertex_info.count) {
        throw std::out_of_range("");
    }

    const VertexAttributeInfo& attr_info = vertex_info.attributes[mesh_loc];
    glVertexArrayAttribBinding(shader_loc, 0);
    glVertexArrayAttribFormat(shader_loc,
                              attr_info.elements,
                              attr_info.type,
                              attr_info.normalized,
                              vertex_info.offsets[mesh_loc]);
    glEnableVertexArrayAttrib(shader_loc);
}

void MeshedBuffer::draw(std::size_t n_triangles, std::size_t first) const {
    auto offset = details::GetIndexSize(index_enum) * first;
    Ctx::Get().glDrawElements(GL_TRIANGLES,
                              3 * n_triangles,
                              index_enum,
                              reinterpret_cast<const void*>(offset));
}